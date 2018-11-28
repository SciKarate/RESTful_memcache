//Josh Reiss and Ryan Neumann
# CS389_HW5

1) Goals + System Boundaries
	System boundaries should be drawn such that our results are not affected by the speed of Reed's network. Therefore, we will run all tests on localhost.
	.
	Our goals are to:
		evaluate the performance of our server
		see how altering various factors affects response time
		emulate the ETC workload
		figure why we randomly segfault from time to time

2) List Services and Outcomes 
	Get - Error code or {key, value}
	Set - Error code or success code + saves k/v pair or horribly breaks
	Del - Error code or success code + deletion
	Msz - {memsize, maxmem}
	Hed - Returns header json
	Shu - Shuts down server

3) Select Metrics/Criteria
	Time, space

4) List Parameters of System and Workload
	- size of server cache
	- requests/second
	- size of requests
	- number of total requests

5) Select Factors That Will Be Varied
	For respsize - vary size of stored values, hold all else constant
	For resptime - vary number of requests per second, hold all else constant

6) Select Evaluation Technique
	Use clock_monotonic_raw to get precise response times.

7) Select Workload
	We emulated the ETC workload, which has the following characteristics:
		There is a 30:1 ratio between gets and sets/"updates"
		There are many deletes, but not nearly as many as gets. (we chose to represent this as a 6:1 delete to set ratio)
		40% of all values are 2 to 11 bytes (we chose to assume that 90% are below 128 bytes, and the rest are less than 1kb)
		Requests come all the time, sometimes with no delay.
		Many deletes attempt to delete keys which no longer exist. (many of our gets are also for keys that DNE)

8) Design Experiments to Maximize Info, Minimize Effort
	Our tests are fairly small, and use loops + randomization + commandline variables to make it easy to run many tests with varied factors.

9-10) Analyze and Interpret Data + Present It
	For resptime -
		The point at which average response time exceeds 1ms is at 245 requests per second.
		This number is found by running 1000 requests and averaging their response time.
		The way in which we make these requests might have some overhead, so we first run our tests without actually making any server requests and time that. We subtract this result from the time our tests take when making server requests, thus theoretically removing any overhead.
		At small numbers of requests/second, this overhead can have a large impact. However, as requests/second increase, the effect of overhead dimishes to the point of being 1/10,000th of the elapsed time at 245 requests per second.
		In other words, overhead increases at a substantially slower rate than total elapsed time.
		It is very likely that the overhead increases linearly while the total elapsed time increases exponentially.
	.
	For respsize -
		resp_size_data.json contains precise data. The keys are the size of requests being made in bytes. The values are the average response time for those sizes in ms. All runs are occuring at 245 requests per second, and thus should lay around 1ms in response time if size has no effect.
		We can see from the data that, for low sizes, there is little to no change in response time. However, response time increases exponentially, and quickly increases towards 17ms at 81000 bytes per value.
		At 82000 bytes per value, the server completely stops responding and shuts down.
	.
	For ETC workload -
		Everything works - usually.
		If you run the workload over and over again, the server segfaults after a while. We do not know why.
		It doesn't seem to be after a particular number of requests, and thus must be random, but it seems to only happen after a very large number of requests, at least.
		We found that running sets of 50-100 sets of requests (about 20,000 requests) was unlikely to result in a segfault, but increasing beyond 100 sets quickly resulted in segfaulting, as did running 100 sets 3-5 times.
		The code now includes a line that shuts down the server at the end of runtime to avoid this behavior.