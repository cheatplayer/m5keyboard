# client

2020 by cp


`\001CMD\002NAME\003DATA\004`

`CMSG FROM DATA`

### cmd

GET/POST

- cmd
- filename
- data

`CMD NAME DATA`


### input

GET/POST

- q

### sdsave

POST

- filename
- text

`SAVE NAME DATA`

### sdload

POST

- filename

`LOAD NAME`
`EXEC NAME`

### sdrm

POST

- filename

`RMRM NAME`

### record

POST

- filename

`RECD NAME`

### recordend

POST

`REND`

### repeat

POST

- filename
- times

`REPT NAME TIME`

### loop

POST

- filename

`LOOP NAME`

### loopend

POST

`LEND`


### sdread

POST

request twice

`READ NAME DATA`

### sdls

POST

request twice

`LSLS ROOT DATA`

### restart

POST
