#define pr_fmt(fmt) "%s: " fmt, KBUILD_MODNAME

#include <linux/pci.h>

MODULE_LICENSE("GPL");

static unsigned char bus = 0;
module_param(bus, byte, 0);

static unsigned char device = 0;
module_param(device, byte, 0);

static unsigned char function = 0;
module_param(function, byte, 0);

int init_module(void) {
  struct pci_dev *dev =
      pci_get_domain_bus_and_slot(0, bus, (device << 3) | (function & 0b111));
  if (!dev) goto error;
  struct pci_bus *pci_bus = dev->bus;
  if (!pci_bus) goto error;
  pr_info("PCI bus 0x%02x:\n", bus);
  struct resource *res;
  unsigned int i;
  pci_bus_for_each_resource(pci_bus, res, i) {
    pr_info("PCI bus resource[%u]: %pR\n", i, res);
  }
  return -EBUSY;
error:
  return -EINVAL;
}
