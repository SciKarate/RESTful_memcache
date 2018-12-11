//Josh Reiss and Ryan Neumann
# CS389_Setup
Run the following commands in order to run and compile files
package download commands:
sudo apt-get install libjsoncpp-dev
sudo apt-get install libpthread-stubs0-dev
sudo apt install libboost-all-dev
sudo apt install libcurl4-openssl-dev

HW6 - Step 1
After HW5, our response time was already below 1ms for GETs at higher than 10 requests per second.

It is worth noting that all our tests were run on localhost.
One could argue that, although this removes overhead caused by Reed's network, it introduces new overhead in that the server and cache might be stealing cycles from eachother.
We have disproved this theory via experiment. On a machine with a 16-thread 4.6GHz CPU and an 8-thread 4.0GHz CPU, there is no difference in performance.
If there were cycles being stolen on the 8-thread 4.0GHz CPU, one would expect a change in performance on the CPU with a higher clock speed and more threads.
Therefore, We believe that we are not encountering any problems with cycle theft.







HW6 - Step 2
On Josh's laptop, we reach the saturation point of 1ms at 277 requests per second with the described workload.

On Ryan's laptop,