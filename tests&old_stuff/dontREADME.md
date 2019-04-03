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

On Ryan's laptop, we reach the saturation point of 1ms at 242 requests per second with the described workload.

Running perf shows -
14.04% overhead for cload.out from [kernel.kallsysms] syscall_return_via_sysret
11.42% overhead for cload.out from [unknown] 0xfffffe000000601b
All other overhead is very minimal.
Surprisingly, our server's overhead never exceeds 0.03% for any item.

Before identifying bottlenecks, we used crow's built-in multithreading.
This means our server is now multithreaded.
Then, to identify these bottlenecks, we tried to see if the following affected their overhead percentages:
-> Removing all random generation, to no effect.
-> Using only one key and one value, to no effect.

We cannot think of what else would be using 11.42% overhead at a seemingly arbitrary memory address.





Odd Note:
	cload.out and cletc.out both segfault after finishing on Ryan's computer every time, but not on Josh's. We suspect that this might be caused by our random string generation, as clresp.out does not segfault and the only substantial differentiator between clresp and cletc/cload is random string generation.
	cload.out and cletc.out leak memory all over the place, but we can't identify the cause for that. We never malloc or new anywhere.




Related files:
	cache_load.cc
	cache_server.cc
	cload.out
	serv.out
	perf.data