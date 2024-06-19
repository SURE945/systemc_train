<style>
table
{
    margin: auto;
}
</style>

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
10. add ignorable extension in gp, implement ARM-v7M local monitor, aim to record FSM.
    the most important 4 element for FSM is **state**, **event**, **action** and **transition**.
    reference doc is ARMv7-M Arichitecture Reference Manual PA3-72.

11. implement a fifo model, the idea comes from lecture Operating System, this fifo model is like a monitor, **ps:** signal change frequently case like this, is not suitable for time decoupling, because every time signal change it should call break quantum in order to get the correct value in time.

|Type|OS|FIFO model|
|--|--|--|
|var|in|uint32_t in|
||out|uint32_t out|
||count|uint32_t count|
|buffer|arry[0, ..., n-1]|uint32_t buffer[3]|
|condition|notfull|full.read()|
||notempty|not_empty.read()|
|wait|notfull.wait|full.write(true)|
||notempty.wait|not_empty.write(false)|
|signal|notfull.signal|full.write(false)|
||notempty.signal|not_empty.write(true)|

12. implement a fifo in model, this fifo uses the concept of semaphore, 11_fifo the fifo is a model

13. implement a round-robin arbiter, the core is a increasing pointer.

14. thread reset feature, thread will restart if condition satisified

    1. reset_signal_is(..,..) only valid at clk.pos() or clk.neg()
    2. async_reset_signal_is(..,..) valid at any wait

15. self implement tlm model

    1. inheritence base abstract class sc_interface to implement abstract template class liushuo_interface;
    2. inheritence liushuo_interface to implement channel model memory;
    3. implement a sc_module master to involve sc_port to link with liushuo_interface;

16. self implement tlm2 b_transport

    1. using sc_export, increase code readable;
    2. add register callback function;

17. sc_mutex, only support atomicity of operation, cannot ensure the sequence of operation, in other words cannot relaize the synchronous of operation.

18. sync case, different master send data at the same time, how to execute in order and ensure the quantum of operation

19. add single port multiple interface case