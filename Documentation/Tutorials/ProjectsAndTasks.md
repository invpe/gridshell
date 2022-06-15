<p align=center>
  <img src=https://camo.githubusercontent.com/67f3d5b189fd0c66ac455e9b421a7c9df2811b31716926e76ec00547165c8438/68747470733a2f2f6772696473686c2e66696c65732e776f726470726573732e636f6d2f323032322f30362f696d675f333438362e706e67>
  </p>
  

# Projects and Tasks
For a grid computing to actually work, we need to provide it's computational power some real work to do.
This work is called Tasks on the GridShell.

Projects are just containers, so it is easier to reffer to a project rather than a task number.

Tasks are exactly what is being executed and computed/**mined** on all GridShell miners and are written in an [BASIC](https://en.wikipedia.org/wiki/BASIC)-like language, leveraging a great open source interpreter called [mybasic](https://github.com/paladin-t/my_basic).

There is a great amount of tutorials and playgrounds for BASIC language, and since it was made to be _Begginers all purpose_ it was adopted for GridShell  to minimise the effort needed to build projects that can be run on the grid.

The speed of the project execution on the Grid is defined by the number of tasks, their computational complexity and available miners online.
Tasks can't run forever therefore a limit of 60 seconds is applied to task execution for every miner. After this limit is exceeded task is considered failed and restarted.

Projects are considered completed once all tasks are executed, however on top of the completion there is a process called validation - that ensures that all completed tasks within the projects have returned valid outputs from their execution.


# Who is creating the projects ?
In the current version of GridShell, tasks and projects are created by the Core-Team.
Moving forward, when the grid network matures enough submitting tasks to the grid will be possible for everyone.

Creating projects and submitting tasks to the grid, requires Shell Tokens - credits that are given to miners for executing the tasks correctly.

# Why are tasks created ?
There are various reasons for why you would like to distribute your computational workload to a grid system.
The most common one the GridShell was approached with are:
 
- Repeatable computations

Generating values for dynamic graphs always requires some time to compute.
Instead of executing this repeatable task on the web server, it can be offloaded to a grid system that will compute and provide results within minutes.

- Offloading  

Moving computation from devices to a grid system to allow more time and space for additional logic.

- Learning

Learning how to program tasks for distributed/paraller computing in a simple way.

 
