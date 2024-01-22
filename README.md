# systemc_train
train usage of systemc tlm2
1.  test systemc lib work
2.  nb_transport
3.  b_transport
4.  apb2, send 3 data sequently
5.  apb3, send 3 data sequently
6.  dmi
7.  dbg interface
8.  add ignorable extension in generic_payload, take AWCACHE or ARCACHE in AXI as example
9.  create a class quantum_control to implement break quantum feature
```
            my_processor    my_peripheral   
0   ms|-------------------------------------
      |         |                  |
      |         V                  V
50  ms|-------------------------------------sc_current_time() = 50ms
      |       | |                  ^
      |       | |                  |
100 ms|       | |                  |        local_quantum_time = 100ms
      |       | |                  |
      |       | V   b_transport()  V
150 ms|-------|-----============>-----------
      |       | | break quantum    ^
      |       | |                  |        sc_time(50, SC_MS)
      |       | V                  v
200 ms|-------|-----------------------------
      |       |  new quantum size = 50ms
      |       |
250 ms|       V predicted quantum size = 200ms
      |
      |
      V
```