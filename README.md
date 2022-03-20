# ArinVisor
A simple Proof of Concept hypervisor for Intel VT-x that virtualizes an already running system.

## Features
- Full vmcs field initialization
- Ept with identity mapping
- WiP ept splitting & hooking

## Basic Info 
Taken from "Intel® 64 and IA-32 Architectures Software Developer’s Manual, Volume 3C: System Programming Guide chapter 23"

Software enters VMX operation by executing a VMXON instruction.
- Using VM entries, a VMM can then enter guests into virtual machines (one at a time). The VMM effects a
VM entry using instructions VMLAUNCH and VMRESUME; it regains control using VM exits.
- VM exits transfer control to an entry point specified by the VMM. The VMM can take action appropriate to the
cause of the VM exit and can then return to the virtual machine using a VM entry.
- Eventually, the VMM may decide to shut itself down and leave VMX operation. It does so by executing the
VMXOFF instruction.

![](https://nixhacker.com/content/images/2019/08/DeepinScreenshot_select-area_20190808194921-1.png)

VMX non-root operation and VMX transitions are controlled by a data structure called a virtual-machine control
structure (VMCS).
Access to the VMCS is managed through a component of processor state called the VMCS pointer (one per logical
processor). The value of the VMCS pointer is the 64-bit address of the VMCS. The VMCS pointer is read and written
using the instructions VMPTRST and VMPTRLD. The VMM configures a VMCS using the VMREAD, VMWRITE, and
VMCLEAR instructions.
A VMM could use a different VMCS for each virtual machine that it supports. For a virtual machine with multiple
logical processors (virtual processors), the VMM could use a different VMCS for each virtual processor.

## References
- [Sparta](https://github.com/omerk2511/Sparta)
- [Hvpp](https://github.com/wbenny/hvpp)
- [Gbhv](https://github.com/Gbps/gbhv)
- [The intel manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)

### Note
The hypervisor is written for the windows platform, tested on latest windows 10 x64 version (at the time of writing)
