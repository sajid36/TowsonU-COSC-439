#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/usb/ch9.h>

#define VENDOR_ID  0x8564  // Your USB vendor ID
#define PRODUCT_ID 0x1000  // Your USB product ID


// Define missing USB interface constants (since they're missing in your headers)
#define USB_CLASS_MASS_STORAGE 0x08
#define USB_SC_SCSI            0x06
#define USB_PR_BULK            0x50

// Called when the USB device is plugged in and matches this driver
static int usb_logger_probe(struct usb_interface *interface, const struct usb_device_id *id) {
    printk(KERN_INFO "usb_logger: USB device inserted (Vendor: %04X, Product: %04X)\n",
           id->idVendor, id->idProduct);
    return 0;
}

// Called when the USB device is removed
static void usb_logger_disconnect(struct usb_interface *interface) {
    printk(KERN_INFO "usb_logger: USB device removed\n");
}

// Match table for supported devices
static struct usb_device_id usb_table[] = {
    { USB_INTERFACE_INFO(USB_CLASS_MASS_STORAGE, USB_SC_SCSI, USB_PR_BULK) },
    {}
};
MODULE_DEVICE_TABLE(usb, usb_table);

// USB driver struct
static struct usb_driver usb_logger_driver = {
    .name = "usb_logger_driver",
    .id_table = usb_table,
    .probe = usb_logger_probe,
    .disconnect = usb_logger_disconnect,
};

// Module init
static int __init usb_logger_init(void) {
    printk(KERN_INFO "usb_logger: Module loaded\n");
    return usb_register(&usb_logger_driver);
}

// Module exit
static void __exit usb_logger_exit(void) {
    usb_deregister(&usb_logger_driver);
    printk(KERN_INFO "usb_logger: Module unloaded\n");
}

module_init(usb_logger_init);
module_exit(usb_logger_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sajid");
MODULE_DESCRIPTION("A simple USB logger kernel module in C");

