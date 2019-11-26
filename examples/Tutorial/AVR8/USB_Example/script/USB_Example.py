import usb.core
import usb.util

# Get the device
Device = usb.core.find(idVendor = 0x0123, idProduct = 0x4567)
if(Device is None):
    raise ValueError("No device found!")

# Set the default configuration
Device.set_configuration(1)

# Get the configuration
Configuration = Device.get_active_configuration()

# Get the device status
Status = usb.control.get_status(Device)

# Output
print(Configuration)
