# Repositorio de los codigos de la práctica verano 2023  UCN
En este repositorio encontradas los codigos creados para los siguientes sensores y actuadores:

- IMU (Inertial Measurement Unit o Unidad de medición inercial)
- LiDAR
- Ultrasonicos (6 unidades)
- Drivers de motores DC (2 unidades)
- Encoders (2 unidades)
- Joystick

## Proposito
El proposito de este repositorio es para el libre acceso de las personas participantes del proyecto a cargo del Dr. Alvaro Prado Romo, como tambien modificar y agregar ficheros para una buena optimizacion de codigo y funcionabilidad de la plataforma robotica.

## Comandos
Independientemente del SO que se utilice pueden ser utilizado en Windows/Linux a traves del cmd o un programa externo como Putty con los siguientes comandos:

- ssh ubuntu@172.16.0.2 
- roscore
- rosrun pwm_controller [nodo del sensor a llamar]

En las carpetas del paquete existe una carpeta llamada `launch` con un fichero de `.launch` con una sintaxis XML con los nodos publicadores para ser lanzados exceptuando el nodo de los actuadores con joystick. En las cuales para añadir un nodo en el fichero se utiliza la siguiente sintaxis.

> `<node name="gps_publisher" pkg="pwm_controller" type="gps_publish.py"/>`

El `type` es el fichero con la codificación del sensor donde el paquete que se indica buscara el fichero, si no lo encuentra, arrojara un error de que no fue encontrado el fichero o en casos que no exista el paquete tambien lo notificará. Para ejecutar con el fichero .launch es la siguiente:

> `roslaunch pwm_controller node_launch.launch`

Al ser un fichero launch, tiene la ventaja de poder editar y añadir más nodos con el proposito de no ocupar muchos terminales a la hora de ejecutar cada uno los nodos publicadores, maximizar y optimizar la carga de datos para la raspberry pi 4 a la hora de consultar a los nodos subscriptores de los sensores implementados en la plataforma robotica movil.