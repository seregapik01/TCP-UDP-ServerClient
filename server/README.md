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
- Data transfer TCP
   - [ ] send/write(...)
- Data transfer UDP
   - [x] recv(...)
   - [x] send(...) **- optional**
- Termination ( int fd )
   - [x] close(fd)
