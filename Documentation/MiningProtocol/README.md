# GridShell mining protocol 🔡

The communication protocol is plain text in the current version of GridShell.

----------------
Miners
----------------

- All miners connect to a central server
- All miners have to heartbeat every **10**s


1. Connect to work.gridshell.net:1911
2. Ident with user-hash, version and mac
`JOB,USERHASH,VERSION,MAC`
3. Heartbeat by sending `PONG` to the server, every 10 seconds
4. Wait for a task to be provided in a format of
`EXEC,BASE64(basscript),BASE64(payload)` 
5. Execute a task with MYBASIC interpreter
6. Once completed return task payload with RESULTS 
`RESULTS,RETCODE,BASE64(results)` 
7. Go to point 3

