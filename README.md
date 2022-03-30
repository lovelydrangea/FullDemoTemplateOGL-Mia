# FullDemoTemplateOGL

Este demo contiene lo necesario para hacer una aplicacion usando OpenGL, contiene librerias para el uso de las extensiones de GL, librerias para soporte de
imagenes y una implementacion primitiva para deteccion de colisiones. Este proyecto intenta implementar los conceptos de POO para facilitar el uso de OpenGL.

Entre los archivos mas destacados se tienen:

| File | Dependency | Contains |  
| ----------- | ----------- | ----------- |   
|Utilities.h|N/A|Struct Ejes, SCR_WIDTH, SCR_HEIGHT, Struct Vertex, Struct Texture, s2ws, loadFile, TextureFromFile, namespace UTILITIES_OGL	(Struct ImageDetails, Struct Vertices, Struct Maya, sumaNormal, normaliza, vectoresEsfera, Esfera, Plano, genNormal)|
|Logger.h|N/A|namespace logger|
|GamePadRR.h|N/A|class GamePadRR|
|material.h|N/A|zeroVec3,	Struct Material, Struct Light|
|shader.h|material.h|class Shader|
|camera.h|Utilities.h|enum Camera_Movement, class Camera	(Friend of MainModel)|
|KDTree.h|N/A|class Node, class KDTree|
|mesh.h|Utilities.h, material.h, shader.h, KDTree.h|class Mesh|
|Billboard.h|Utilities.h, camera.h, shader.h|class Billboard|
|Model.h|Utilities.h, material.h, shader.h, mesh.h, camera.h, KDTree.h|class Model (Friend MainModel)|
|MainModel.h|Utilities.h,Model.h, camera.h|class MainModel	extends of Model and Camera|
|SkyDome.h|Utilities.h, Model.h, camera.h, shader.h|class SkyDome	extends of Model|
|Terreno.h|Utilities.h, Model.h, camera.h, shader.h|class Terreno extends of Model|
|CollitionDetection.h|KDTree.h|SolveEquision,	checkCollision,	print_queue, findCollision|
|Scenario.h|camera.h, Model.h, MainModel.h, SkyDome.h, Terreno.h, Billboard.h, CollitionDetection.h|class Scenario|
|KeyboardInput.h|Scenario.h, camera.h| Struct Input	(enum Keys),	KEYS,	KeysEvents,	Init)
  
   
Esta plantilla esta estructurada para poder cargar modelos genericos y convencionales de forma rapida y administrada, se pueden generar escenarios individuales
extendiendo la clase Scenario o bien duplicandola con su logica respectiva, se podría utilizar una interface o clase abstracta para Scenario pero esta como work in progress.
  
La plantilla por defecto tiene en su carpeta de binarios la libreria opengl32.dll para forzar el render por medio de Mesa en lugar de usar la 
grafica de la computadora, si tu grafica soporta las extensiones de OGL 2 en delante, puedes remover el archivo, si necesitas extensiones superiores de OGL
te invito a modificar el archivo principal del WinMain para agregar las extensiones faltantes a tu ventana.  

Mejoras o bugs, favor de levantar un issue.
