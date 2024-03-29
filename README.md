# PARAM_PROCESSCPP

El branch de master contiene las modificaciones estables del software. </br>
El branch de debug contiene las ultimas modificaciones, por ende es mas inestable.

/*compile: g++ -o run main.cpp -ID:\PROGRAMAS\opencv\build\include -LD:\PROGRAMAS\opencv\build\bin -llibopencv_core450 -llibopencv_highgui450 -llibopencv_imgcodecs450 -llibopencv_imgproc450 -llibopencv_photo450 -llibopencv_video450 -llibopencv_videoio450*/

<html>
    <head>
        <meta charset="UTF-8">
    </head>
    <body>
         <h1>INDICE</h1>
        <ul>
            <li><a href="#descripción">Descripción</a>
                <ul>
                    <li><a href="#--dependencias--">Dependencias</a></li>
                    <li><a href="#--compilación-y-ejecución-del-programa--">Compilación y ejecución del programa</a></li>
                </ul>
            </li>
            <li> <a href="#como-compilar-opencv">Como compilar opencv</a>
                <ul>
                    <li><a href="#--prerrequisitos--">Prerrequisitos</a></li>
                    <li><a href="#--configuración-del-make--">Configuración del cmake</a></li>
                    <li><a href="#--compilación-e-instalación--">Compilación e instalación</a></li>
                    <li><a href="#--configuración-del-path--">Configuración del path</a></li>
                </ul>
            </li>
        </ul>
        <h1>DESCRIPCIÓN</h1>
        Este proyecto de la rama de visión por computadora es capaz de detectar paramecium tetraurelia y obtener su centroide y angulo para poder trazar su trayectoria y calcular su torque y velocidad de nado. Su funcionamiento es a base de operaciones morfologicas en las imagenes impuestas por el usuario.
        </br>
        <img src="./resources_readme/2021-07-06 21-34-52.gif"></img>
        <h2>  Dependencias  </h2>
            <ul>
                <li>Opencv 4.5.0</li>
            </ul>
        <h2>  Compilación y ejecución del programa  </h2>
        Compilación: <i>./compile.bat</i> <b>NOTA: En este script puede variar en donde esta instalado opencv, el parametro I y el parametro L se debe de poner la ruta respectiva de tanto los includes como los lib <i>(-I{extract_directory}/opencv/build/include && -L{extract_directory}/opencv/build/lib )</i></b> </br> </br>
        <b>Ejecución: <i>Usage: run.exe input [params]</br>
        -?, -h, --help</br>
                print the help for this program</br>
        -d, --delay (value:1)</br>
                set the fps or delay in video</br>
        --fN, --file_name (value:No_Name)</br>
                string for data files</br>
        -s, --save </br>
            set a flag indicate if the program generate a data file</br>
        input</br>
                input video in wmv format</i></b></br>
        <h1>COMO COMPILAR OPENCV</h1>
        <h2>  Prerrequisitos  </h2>
        <img src="./resources_readme/req.png"></img>
        <p>Descarga <a href="https://www.dropbox.com/sh/oqlm559dbgpkjnh/AADK-Qc6eJYXH9d-indPd0nBa?dl=0" target="_blank"> aquí. </br></a>
        <h2>  Configuración del make  </h2>
        Una vez todo descargado se procede a instalar el cmake y el mingw, despues se ejecuta el opencv 4.5.0 y se extrae en donde se quiera. </br></p>
        <img src="./resources_readme/op.png"></img>
        Despues se entra a la carpeta que se extrajo la cual es opencv. Adentro se mostrara esta ventana. <b>NOTA: Si el directorio build no esta se debera crear manualmente.</b>
        <img src="./resources_readme/dir.png"></img>
        Despues se abre el cmake previamente instalado y de debera configurar primero de la siguiente manera.
        <img src="./resources_readme/cm1.png"></img>
        Despues se le da al boton configure y abrira una ventana la cual se debera de configurar así, despues se le da a next.
        <img src="./resources_readme/cm2.png"></img>
        Pedira los compiladores a usar. Se debera meter la siguiente ruta para c y c++:  {instalation_path}/mingw-w64/mingw64/bin/x86_64-w64-mingw32-gcc.exe <-- para C y {instalation_path}/mingw-w64/mingw64/bin/x86_64-w64-mingw32-g++.exe <-- para c++. Despues se le da finalizar
        <img src="./resources_readme/cm3.png"></img>
        Una vez finalizado se debera de deshabilitar el OPENCV_ENABLE_ALLOCATOR_STATS
        <img src="./resources_readme/cm5.png"></img>
        Despues se le da a configurar y cuando termine el proceso se le data a generar. Una vez generado el archivo make se puede cerrar cmake-gui.</br>
        <h2>  Compilación e instalación  </h2>
        Para la compilación se debe abrir una terminal en la carpeta {extract_directory}/opencv/build y ahi ejecutar el comando <i>mingw32-make.exe</i> y comenzara a compilar todo lo necesario para funcionar. Esto puede tardar varios minutos dependiendo el procesador que se cuente.
        <img src="./resources_readme/m1.png"></img> 
        El resultado debera de ser similar al de la imagen. </br>
        Despues de que termine la compilación se debe de ejecutar el comando <i>mingw32-make.exe install</i> para instalar la libreria.
        <h2>  Configuración del path  </h2>
        Abra las variables de entorno del sistema y configure en la variable path la ruta {extract_directory}/opencv/build/bin.
        <img src="./resources_readme/m2.png"></img> 
    </body> 
</html>