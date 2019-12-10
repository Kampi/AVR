import usb.core
import usb.control

# Device specific vendor and product id
Vendor = 0x0123
Product = 0x4567

# List all connected devices
Devices = usb.core.show_devices()
if(Devices is None):
    raise ValueError("No devices found!")

# Get the target device
Device = usb.core.find(idVendor = Vendor, idProduct = Product)

# Set the first configuration
Device.set_configuration(1)

# Get the active configuration
Config = Device.get_active_configuration()[(0, 0)]

# Set the first interface
# Doesn´t work atm
#usb.control.set_interface(Device, 1, 0)

# Get the OUT endpoint
EP_OUT = usb.util.find_descriptor(Config,
                                  custom_match = lambda e: \
                                  usb.util.endpoint_direction(e.bEndpointAddress) == \
                                  usb.util.ENDPOINT_OUT)

# Get the IN endpoint
EP_IN = usb.util.find_descriptor(Config,
                                  custom_match = lambda e: \
                                  usb.util.endpoint_direction(e.bEndpointAddress) == \
                                  usb.util.ENDPOINT_IN)

EP_OUT.write("Hello")

# Display the target information
print("Device:\n\r{}".format(Device))
print("Product: {}".format(Device.product))
print("Manufacturer: {}".format(Device.manufacturer))
print("Serial number: {}".format(Device.serial_number))
for n, ID in enumerate(Device.langids):
    print("LANG {}: 0x{:02x}".format(n, ID))
print("Status: {}".format(usb.control.get_status(Device)))
print("Current configuration:\n\r{}".format(Config))
print("Interface: {}".format(usb.control.get_interface(Device, 0)))
