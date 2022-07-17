# GridShell mining protocol 🔡

The communication protocol is plain text in the current version of GridShell.

----------------
Miners
----------------

- All miners connect to a central server
- All miners have to heartbeat every **10**s


1. Connect to work.gridshell.net:1911
2. Obtain Server public key, P, G values from the welcome message

`Welcome - Welcome string
184440799 - Server P
3 - Server G
148722804 - Server Public Key`

3. Calculate miner Public, Private key and symmetric key
4. `SHA1(symmetric_key)`
5. `XOR(user_hash with SHA1(symmetric_key))`
6. `BASE64ENCODE(XOR(user_hash, SHA1(symmetric_key)))`
7. Send `JOB,miner_public_key,cipher,VERSION,MAC`
8. Heartbeat by sending `PONG` to the server, every 10 seconds
9. Wait for a task to be provided in a format of
`EXEC,BASE64(basscript),BASE64(payload)` 
10. Execute a task with MYBASIC interpreter
11. Once completed return task payload with RESULTS 
`RESULTS,RETCODE,BASE64(results)` 
12. Go to point 9

