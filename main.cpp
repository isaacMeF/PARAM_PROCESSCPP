
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <ctime>
#include <deque> 

#define M_PI 3.14159265358979323846

int main(int argc, char** argv){

    const std::string keys_args = "{help h ?   |           | print the help for this program}"
                                  "{@input     |           | input video in wmv format}"
                                  "{fN file_name |No_Name| string for data files }"
                                  "{delay d    | 1 | set the fps or delay in video}"
                                  "{save s |      | set a flag indicate if the program generate a data file  }";

    cv::CommandLineParser parser( argc, argv, keys_args ); //Parser para obtener y leer el video desde los argumentos de ejecución del programa
    std::string x_data_name = ".\\data_files\\x_";
    std::string y_data_name = ".\\data_files\\y_";
    std::string angle_data_name = ".\\data_files\\angle_";

    if(parser.has("help") || parser.has("?") || parser.has("h")){
        parser.printMessage();
        return 0;
    }
    if(!parser.check()){
        parser.printErrors();
        parser.printMessage();
        return 0;
    }
    bool hasDataFile = parser.has("s") || parser.has("save") ? true : false;
    x_data_name += parser.get<std::string>("file_name");
    x_data_name += ".csv";
    y_data_name += parser.get<std::string>("file_name");
    y_data_name += ".csv";
    angle_data_name += parser.get<std::string>("file_name");
    angle_data_name += ".csv";



    std::clock_t start = std::clock();
    cv::VideoCapture video;
    cv::Mat frame, frameGray, NormalizeFrame, thFrame, DilFrame, cannyFrame; //Definiendo matrices vacias
    cv::Mat kernel;
    kernel.ones(cv::Size(10,10), CV_8UC1);

        std::ofstream x(x_data_name);
        std::ofstream y(y_data_name);
        std::ofstream angle(angle_data_name);
    
  

    //std::cout<<start<<std::endl;

    cv::namedWindow("MainWindow", cv::WINDOW_GUI_EXPANDED);
    cv::namedWindow("GrayWindow", cv::WINDOW_GUI_EXPANDED);
    cv::namedWindow("NormalizedWindow", cv::WINDOW_NORMAL);
    cv::namedWindow("thWindow", cv::WINDOW_NORMAL);
    cv::namedWindow("DWindow", cv::WINDOW_NORMAL);
    cv::namedWindow("EWindow", cv::WINDOW_NORMAL);
    cv::namedWindow("OWindow", cv::WINDOW_NORMAL);
    cv::namedWindow("CannyWindow", cv::WINDOW_NORMAL);

    //std::string strCoords;
    std::vector<std::vector<cv::Point>> cnts; //[x,y]
     //Carga el input en el objeto video capture (se puede modificar para analizar paramecium en tiempo real con alguna camara como el dinolite) 
    if(parser.get<std::string>( "@input" ) == "0" || parser.get<std::string>( "@input" ) == "1" || parser.get<std::string>( "@input" ) == "2" ){
        video = cv::VideoCapture(parser.get<int>( "@input" ));
    }else{
        video = cv::VideoCapture(cv::samples::findFile(parser.get<std::string>( "@input" ) ));
    }
    
    int total_frames = video.get(cv::CAP_PROP_FRAME_COUNT);
    int fps = video.get(cv::CAP_PROP_FPS);
    
    std::cout<<"FILE: "<<parser.get<std::string>("@input")<<std::endl;
    std::cout<<"RESOLUTION: "<<"["<<video.get(cv::CAP_PROP_FRAME_HEIGHT)<<"x"<<video.get(cv::CAP_PROP_FRAME_WIDTH)<<"]"<<std::endl;
    std::cout<<"FPS: "<<fps<< std::endl;  
    if(!hasDataFile){
        x.close();
        y.close();
        angle.close();
        std::cout<<"SAVE MODE: OFF \n";
        std::string global_inst("del /f ");
        std::string instruccionx, instrucciony, instruccionangle;
        instruccionx = global_inst + x_data_name;
        instrucciony = global_inst + y_data_name;
        instruccionangle = global_inst + angle_data_name;
        std::system(instruccionx.c_str());
        std::system(instrucciony.c_str());
        std::system(instruccionangle.c_str());
     
    }else{
        std::cout<<"SAVE MODE: ON \n";
    }

    if(!video.isOpened()){
        std::cout<<"No existe el video solicitado"<<std::endl;
        return 0;
    }
    

    int count = 0;
    while(true){
        video.read(frame);
        if(frame.empty()) break; 

        //preprocessing part
        cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY); //Converción de canal BGR a GRAY
        cv::normalize(frameGray, NormalizeFrame, 0, 255, cv::NORM_MINMAX); //Normalización del frame entre valores de 0 a 255 por el metodo de MINMAX
        cv::threshold(NormalizeFrame, thFrame, 50, 255, cv::THRESH_BINARY); //Binarización del frame, con un valor de thresh de 60 y un maxval de 255
        cv::dilate(thFrame, DilFrame, cv::getStructuringElement(cv::MORPH_ELLIPSE,(cv::Size(6,6))));//Dilatación de la imagen con una estructura especificada en tamaño y forma para la operacion morfologica.
        cv::imshow("DWindow", DilFrame);
        //MORPH_ELLIPSE es un elemento estructural eliptico el cual esta inscrito en un rectangulo[x,y,width,heigth].
        cv::erode(DilFrame, DilFrame, kernel, cv::Point(-1,-1), 2, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue()); //Esta parte es la erosión de la imagen previamente dilatada. lo hace con una kernel de unos;
        cv::imshow("EWindow", DilFrame);
        //El punto (-1,-1) indica que el punto de ancla de la eroción esta en el centro de cada elemento y este copia el borde original {PENDIENTE cv::morphologyDefaultBorderValue()}
        cv::morphologyEx(DilFrame, DilFrame, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_ELLIPSE,(cv::Size(10,10)))); //Dilata la imagen y luego erosiona la imagen dilatada
        cv::imshow("OWindow", DilFrame);

        //fin preprocessing
        
        //contornos
        cv::Canny(DilFrame, cannyFrame, 50, 200); //Encuentra los bordes de una imagen por medio del algoritmo canny
        cv::findContours(cannyFrame, cnts,cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);  //En base a la imagen procesada con canny este busca los puntos de los bordes y los almacena en un vector de vectores de puntos

        std::vector<std::vector<cv::Point>> PolyAprox(cnts.size());
        std::vector<std::vector<cv::Point>> Hull(cnts.size());
        std::vector<std::vector<cv::Point>> Defects(cnts.size());

        //cv::drawContours(frame, cnts, -1, cv::Scalar(0,0,255));
    
        std::vector<cv::RotatedRect> minEllipse(cnts.size()); //Con los puntos previamente calculados aproxima un rectangulo rotatorio (con el fin de calcular y aproximar la elipse con su respectivo angulo de rotación)
        for(int i = 0; i < cnts.size(); ++i){
            
            cv::approxPolyDP(cv::Mat(cnts[i]), PolyAprox[i], 6.6, false);
            cv::convexHull(cv::Mat(cnts[i]), Hull[i], false);

            minEllipse[i] = cv::minAreaRect(cnts[i]); //Calcula e inscribe el area mas pequeña del rectangulo en donde se inscribira la elipse
            if(cnts[i].size() > 5){
                minEllipse[i] = cv::fitEllipse(cnts[i]); //Inscribe la elipse en el rectangulo
            }
        }
        cv::putText(frame, cv::format("Frame: %d/%d", count+1, total_frames), {frame.rows+25, 25}, 1, 2, cv::Scalar(100,0,255),3,8); //Muestra los frames totales y el numero de frame en el que va el video
        cv::putText(frame, cv::format("FPS: %d", fps), {frame.rows+25, 50}, 1, 2, cv::Scalar(100,0,255),3,8); //Muestra los frames por segundo (FPS)
        for(int i=0; i<minEllipse.size(); ++i){
            if(minEllipse[i].size.area() <3500){
                cv::ellipse(frame, minEllipse[i], cv::Scalar(0,0,255),2);//Pinta la elipse en pantalla
                cv::putText(frame, cv::format("No. Paramecium: %ld", minEllipse.size()), {10,25}, 1,2, cv::Scalar(0,255,255),3, 8); //Pinta el numero de paramecios totales en pantalla
                cv::drawMarker(frame, minEllipse[i].center, cv::Scalar(0,0,255), 0,10); // Pinta una cruz en el centro del paramecio
                double realAngle = minEllipse[i].angle;  
               
                cv::putText(frame, cv::format("([%.3f, %.3f], %.2f deg)",minEllipse[i].center.x,minEllipse[i].center.y, realAngle) ,
                        minEllipse[i].center, 1 ,1.3,cv::Scalar(255,255,100),2, cv::LINE_AA); // Pinta las coordenadas (x,y) y el angulo del paramecio   
                           
                cv::circle(frame, minEllipse[i].center, 6, cv::Scalar(0,255,0),2, cv::LINE_8);    

               //cv::drawContours(frame, PolyAprox, i, cv::Scalar(255,255,0), 2);

                

                

                //std::cout<<i<<": "<<PolyAprox[i]<<std::endl; 

            

                //std::cout<<"CENTROS EN EL "<<count<<" FRAME: "<<minEllipse[i].center<<std::endl;
                
                //cv::line(frame, centers[0], centers[1], cv::Scalar(100,200,255), 10, cv::LINE_8, 0);

                /*
            //std::cout<<minEllipse[i].center;
                std::cout<<"PUNTOS DE CONTORNO:"<<std::endl;
                std::cout<<i+1<<": "<<cnts[i]<<std::endl;
                std::cout<<"ELLIPSE SIZE (a y b), center & angle"<<std::endl;
                std::cout<<minEllipse[i].size<<std::endl;
                std::cout<<minEllipse[i].center<<std::endl;
                std::cout<<minEllipse[i].angle<<std::endl;*/
            

                //std::cout<<minEllipse[i].size.area()<<std::endl;
            if(hasDataFile){
                x<<minEllipse[i].center.x<<",";
                y<<minEllipse[i].center.y<<",";
                angle<<realAngle<<",";
            }
             

            

            }
        }
 

        /*for(int i = 0; i<frame.rows; ++i){
            uchar* rowsFrame = frame.ptr<uchar>(i);
            for(int j = 0; i<frame.cols; ++j){
                std::cout<<(int)rowsFrame[j]<<std::endl;
            }
        }*/
        
        //Muestra todo el proceso y el resultado final en frame
        cv::imshow("MainWindow", frame);
        cv::imshow("NormalizedWindow", NormalizeFrame);
        cv::imshow("thWindow", thFrame);
        //cv::imshow("DWindow", DilFrame);
        cv::imshow("GrayWindow", frameGray);
        cv::imshow("CannyWindow", cannyFrame);   

        if(cv::waitKey(parser.get<int>("delay")) == 27){ //condicion de paro del video, si se aprieta la tecla esc para el video
            break;
        }
        if(hasDataFile){
            x<<"\n";
            y<<"\n";
            angle<<"\n";
        }
        
        ++count; //contador de frames
    }
    x.close();
    y.close();
    angle.close();
    video.release(); //cierra el video
    cv::destroyAllWindows(); //destruye todas las ventanas creadas

    return 0;
}