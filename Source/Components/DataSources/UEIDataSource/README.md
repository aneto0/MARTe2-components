# Using UEIDAQ DataSource
UEIDAQ DataSource is a MARTe2 component allowing to interface and configure directly from a MARTe application a [UEIDAQ](https://www.ueidaq.com/) device installed both as a remote device (via Ethernet UDP) or as a local device (with MARTe application running directly in the embbedded Linux OS (UEIPAC) on a UEIDAQ chassis) via the PowerDNA API.

This brief manual describes the basic operation of the DataSource and how to configure a data acquisition process through a UEIDAQ device

# Compilation dependencies
The UEIDAQ compiler (PPC) and libraries are available in the CODAC NAS (/mnt/nas-syno2/45CODAC-Common/3rdPartySoftware/UEI/ from srv03).

## DataSource configuration structure
The UEIDAQ DataSource component itself is a generic datasource which can read and write to DAQ Memory Maps configured for a certain device in MARTe and also acquire single data points via Point-by-Point data acquisition mode.

In order to use the UEIDataSource, the first step is to configure and initialise the UEIDAQ device. In order to do so, the MARTe application configuration class must define a UEIMasterObject instance, which is in charge of the management, configuration process and DAQ Maps definition for a given UEIDAQ device.

A basic example of the definition of such object is given below:
```
+UEIDevice1 = {
	Class = UEIMasterObject
	Ip = {127, 0, 0, 2}
	Port = 6334
	+Devices = {
		Class=ReferenceContainer
		+dev0={
			Class = UEIAI217_803
			Devn = 0
			Gains = {1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
			Sampling_ferquency = 1000.0
		}
		+dev1={
			Class = UEIAI217_803
			Devn = 1
			Gains = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
			Sampling_ferquency = 1000.0
		}
	}
	+Maps = {
		Class=ReferenceContainer
		+Map1 = {
			Class = UEIRtDMap
			ScanRate = 1.0
			Outputs = {
				dev0 = {
					Devn = 0
					Channels = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}
				}
				dev1 = {
					Devn = 1
					Channels = {7, 8}
				}
			}
		}
	}
}
```
