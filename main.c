#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDKeys.h>

int main() {
    printf("Initializing usage-page aware Apple IOKit USB framework...\n");
    IOHIDManagerRef hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    if (!hidManager) {
        printf("Error: Failed to initialize Apple IOHIDManager.\n");
        return 1;
    }

    CFMutableDictionaryRef matchingDict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    int vendorId = 0x046D;
    int productId = 0xC52F;
    int usagePage = 0xFF00; 
    
    CFNumberRef vNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &vendorId);
    CFNumberRef pNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &productId);
    CFNumberRef uPage = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usagePage);
    
    CFDictionarySetValue(matchingDict, CFSTR(kIOHIDVendorIDKey), vNum);
    CFDictionarySetValue(matchingDict, CFSTR(kIOHIDProductIDKey), pNum);
    CFDictionarySetValue(matchingDict, CFSTR(kIOHIDDeviceUsagePageKey), uPage);
    
    IOHIDManagerSetDeviceMatching(hidManager, matchingDict);
    IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);

    CFSetRef deviceSet = IOHIDManagerCopyDevices(hidManager);
    if (!deviceSet || CFSetGetCount(deviceSet) == 0) {
        printf("Error: Logitech Nano configuration interface [0xc52f / 0xff00] not found.\n");
        printf("Please ensure the receiver is plugged directly into a built-in USB port.\n");
        return 1;
    }

    CFIndex deviceCount = CFSetGetCount(deviceSet);
    IOHIDDeviceRef* devices = malloc(sizeof(IOHIDDeviceRef) * deviceCount);
    CFSetGetValues(deviceSet, (const void **)devices);

    IOHIDDeviceRef targetDevice = devices[0];
    printf("Connected successfully to configuration receiver channel via Usage Page 0xFF00.\n");
    printf("Sending raw hardware pairing instruction...\n");

    uint8_t packet[] = {0x10, 0xFF, 0x80, 0xB2, 0x01, 0x00, 0x14};
    
    IOReturn result = IOHIDDeviceSetReport(targetDevice, kIOHIDReportTypeOutput, 0x10, packet, sizeof(packet));

    if (result != kIOReturnSuccess) {
        printf("Error: macOS rejected the write frame. Code: 0x%08x\n", result);
        free(devices);
        return 1;
    }

    printf("\n>>> ACTIVE PAIRING MODE ENABLED <<<\n");
    printf("Immediately toggle the power switch on the bottom of your M525 mouse OFF and back ON now.\n\n");

    for (int remaining = 20; remaining > 0; remaining--) {
        printf("\rTime remaining: %d seconds... ", remaining);
        fflush(stdout);
        sleep(1);
    }

    printf("\nClosing native channel loop. Pairing sequence completed.\n");
    free(devices);
    return 0;
}
