<p align=center>
  <img src=https://gridshl.files.wordpress.com/2022/06/img_3490.png>
</p>

-------------------

# User Performance Factor
The UPF "User Performance Factor" is a value that tells the Grid system how well miners for a user are performing their job.
As we know, the main job of a miner is to mine the value - execute a given task and return output.

To calcualte the performance of every user, the GridShell is looking at two very important numbers:

- Number of tasks a miner returned that are valid
- Number of tasks a miner returned that are invalid

This as you already know, is part of the Validation process that is crucial to the success of project execution.

Every user on the grid has UPF value calculated and presented, this is a score of the user and it is rendering its overall performance factor for the system.
When a validation process takes place, it checks the UPF of the user and decides wether it is required to run a validation or not for a specific task.

Users with low UPF will have their tasks validated once their miners complete the work.
This is because the UPF tells the system, that their miners are returning wrong results.

Users with high UPF are considered trusted and their tasks will not be validated boosting the project completion.
This is because the UPF tells the system, that their miners are returning valid results.

The GridShell performance is tightly coupled with the users UPF values, the higher the UPF's the faster the Grid as the number of validation reduces.

There will be more logic introduced for low UPF users within upcoming [Grid Improvement Proposals](https://github.com/invpe/gridshell/tree/main/GIP).

