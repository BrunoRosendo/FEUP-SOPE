# **Application Server**

Para a segunda entrega do projeto de SOPE (Sistemas Operativos), o objetivo é criar uma aplicação capaz de gerir e executar pedidos de excecução entre um cliente e um servidor. Com efeito, foi necessário recorrer a programas multithread, promover a intercomunicação entre processos através de FIFOs e evitar conflitos entre as entidades envolvidas atrvés de mecanismos de sincronização. 

Deste modo, as tarefas pedidas pelo cliente C são executadas por um código numa biblioteca B (externa) e um servidor S (multithread) que gere esses mesmos pedidos e devolve os resultados. 

Numa primeira fase, pretende-se trabalhar com o componente "Cliente", assumindo que já existe um servidor implemenado (FIFO). Assim, os pedidos de tarefas são enviados para S por um processo C (multithread), ficando armazenados numa fila até serem requisitados e enviados para B, e o resultado guardado num buffer. Note-se que este buffer funciona como um armazém, monitorizado por um thread Consumidor, que envia os resultados, um a um, para C. 


# **Compilation**

To compile simply do make on the src folder.
> make
 
# **Running**

In order to use the client, we need to have the server running aswell.

**Client**

To run the client, call the executable, followed by the running time (seconds) and the public fifo name.
For example.
> ./c -t 1 aFifo

In a more general way

> ./c <-t nsecs> <fifoname>
 
 ```
 ```
 
**Server**
 
To run the server, call the executable, followed by the running time (seconds), size of the buffer and the public fifo name.
For example.
> ./s -t 1 -l 1000 aFifo

In a more general way

> ./s <-t nsecs> [-l bufsz] <fifoname>
 
 
# **Errors and input**

There must always be exactly 4 arguments (including the program call).

Failure to provide a valid flag (-t) or time will yield an error.

Failure to provide a name for the fifo will yield an error.
  
# **Implementation**
  
1. Parse the command arguments
2. Synchronize with the server
3. Generate requests by creating request threads and sending them through the public fifo
4. Create private fifo and wait for the response from the server, which will be sent through this newly created channel.

During this steps, the client will also register operations such as:
* Client request thread makes initial request (IWANT)
* Client request thread acknowledges the arrival of the response from the server (GOTRS)
* Client request thread informs that the request was rejected, because the server was closed (CLOSD)
* Client request thread informs that he can no longer wait for the answer (GAVUP)

```
```

 # **Self-evaluation**
  
Henrique Sousa up201906681: 25%

Domingos Santos up201906680: 25%

Bruno Rosendo up201906334: 25%

João Mesquita up201906682: 25%
