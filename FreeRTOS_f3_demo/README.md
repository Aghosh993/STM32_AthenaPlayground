# F3 Discovery FreeRTOS Queue Demo

This app shows a simple proof-of-concept of how to use FreeRTOS queues to perform IPC (inter-process communication) between two threads that are managing different hardware resources. In this case, a button is sampled at 100 Hz, and a queue forms a FIFO data pipe to relay the button state (an integer) to a second task that turns an LED on or off accordingly.

(c) Abhimanyu Ghosh, 2017