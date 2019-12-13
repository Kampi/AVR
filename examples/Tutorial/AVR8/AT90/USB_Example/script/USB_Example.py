import math
import usb.core
import usb.control

# Device specific vendor and product id
Vendor = 0x0123
Product = 0x4567

if(__name__ == "__main__"):
    # List all connected devices
    Devices = usb.core.show_devices()
    if(Devices is None):
        raise ValueError("[ERROR] No devices found!")

    # Get the target device
    Device = usb.core.find(idVendor = Vendor, idProduct = Product)

    # Set the first configuration
    Device.set_configuration(1)

    # Get the active configuration
    Config = Device.get_active_configuration()[(0, 0)]

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
    print("[DEBUG] Device:\n\r{}".format(Device))
    print("[DEBUG] Product: {}".format(Device.product))
    print("[DEBUG] Manufacturer: {}".format(Device.manufacturer))
    print("[DEBUG] Serial number: {}".format(Device.serial_number))
    for n, ID in enumerate(Device.langids):
        print("[DEBUG] LANG {}: 0x{:02x}".format(n, ID))
    print("[DEBUG] Status: {}".format(usb.control.get_status(Device)))
    print("[DEBUG] Current configuration:\n\r{}".format(Config))
    print("[DEBUG] Current interface: {}".format(usb.control.get_interface(Device, 0)))

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
            ConversionResult = (Analog[1] << 0x08) | Analog[0]
            print(" Conversion result: {}".format(ConversionResult))

            # Get the voltage value of the ADC
            U = 2.56 / 1024 * int(ConversionResult)

            # Print the analog result according to the input channel
            if(int(Channel) == 0):
                # Calculate the resistor value of the NTC
                RT = 100000.0 / (3.3 - (U)) * U

                # Calculate the temperature
                Beta = 4250
                T0 = 298
                R0 = 100000
                T = (Beta / (math.log(RT / R0) + (Beta / T0))) - 273
                print(" Temperature: {:.2f}°C".format(T))
            elif(int(Channel) == 3):
                Battery = (U * 320 / 100) + 0.7
                print(" Battery: {:.2f} V".format(Battery))
            else:
              print("   Analog value: {:.2f}".format(U))
        except Exception as e:
            print("[ERROR] Invalid Input!")
            print("[ERROR] Exception: {}".format(e))
