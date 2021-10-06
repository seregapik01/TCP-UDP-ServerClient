:notebook: Server API Overview\
 **TCP/UDP** :left_right_arrow: 
- Initialization
   - [x] socket(...)
   - [ ] setsockopt(...) **- optional**
   - [x] bind(...)
   - [x] listen(...)
- Connection TCP
   - [ ] connect(...) **- optional**
   - [X] accept(...)
- DataTransfer TCP
   - [ ] send/write(...)
- DataTransfer UDP
   - [x] recv(...)
   - [x] send(...) **- optional**
- TraceSockects
   - [X] select(...) 
- Termination ( int fd )
   - [x] close(fd)
