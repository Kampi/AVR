import usb.core
import usb.control

# Device specific vendor and product id
Vendor = 0x0123
Product = 0x4567

if(__name__ == "__main__"):
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

    while(True):
        try:
            # Get the channel
            Channel = input("[INPUT] Please enter the ADC channel: ")

            # Check if the channel is in range
            if(int(Channel) > 7):
                raise Exception

            # Transmit the channel
            Device.write(EP_OUT.bEndpointAddress, Channel)

            # Get the analog value from the ADC
            Analog = Device.read(EP_IN.bEndpointAddress, 2)

            print("[INFO] Analog value: 0x{:02X}".format((Analog[1] << 0x08) | Analog[0]))
        except Exception as e:
            print("[ERROR] Invalid channel!")
