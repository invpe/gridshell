<p align=center>
<img src=https://gridshl.files.wordpress.com/2022/06/img_3489.png>
</p>

------------------------
# Validation
When a task is executed the output of the calculation (mining) is returned to the owner of the project.
This person is waiting for this data and in majority of the cases it is very important for him for this data to be valid.
Some of the tasks running on the GridShell are rendering the harness of the air quality to their users, therefore the mined data
needs to be 100% correct.

In order for this correctnes to be in place, GridShell is using replication validation, which is nothing else than running the task again,
on miners that are called **validators**.

When **validators** complete their work, the system compares results between **validator** and **miner** who completed the task.

- If results are matching, the task results are considred to be correct - and the owner of the miner executing this task is rewarded with a Shell token.
- If results are different, another round of execution is decided and process repeats.

This is ensuring that no miner can return noise in their output and that each task is validated for its corretness.

Validators play key role in the confirmation of the results of tasks execution.

