# project-9-robert-eva-emma
project-9-robert-eva-emma created by GitHub Classroom
##1.What is the output that you received from running part 1?

    Process 0 sending 10 to 1, tag 201 (4 processes in ring)
    Process 0 sent to 1
    Process 0 decremented value: 9
    Process 0 decremented value: 8
    Process 0 decremented value: 7
    Process 0 decremented value: 6
    Process 0 decremented value: 5
    Process 0 decremented value: 4
    Process 0 decremented value: 3
    Process 0 decremented value: 2
    Process 0 decremented value: 1
    Process 0 decremented value: 0
    Process 0 exiting
    Process 1 exiting
    Process 2 exiting
    Process 3 exiting

##2.Show the plot of average execution times and standard deviation from part 

![Average Runtime](https://github.com/daemon-deacons/project-9-robert-eva-emma/blob/master/Avg_p2.png)
![Standard deviation of Runtime](https://github.com/daemon-deacons/project-9-robert-eva-emma/blob/master/SD_p2.png)
![Runtime boxplot](https://github.com/daemon-deacons/project-9-robert-eva-emma/blob/master/boxplot.png)

 ##2.a.Why do you think there is a difference in the execution times?
 
In the first variant,we only use TCP protocol for every process.
In the second variant, it allows the MPI processes on same nodes
to use shared memory for their intercommunication, and MPI processes on 
different nodes to use TCP. Comparatively, the second variant is faster 
than the first one.
Finally, the TCP overhead between the nodes is reduced in the third variant 
by using the USNIC interconnect. It makes the third variable fastest among the
three variables.
    

##3.Show the plot of average execution times and standard deviation from part 
    ##3.a.Why do you think there is a difference in the execution times?
