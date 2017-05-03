# Meeting notes realtek Kylin SVP

Buffers are protected, TEE is running on separate secure processor.
The REE receives / fetches encrypted content snippets, and passes these to the TEE.

How are credentials passed to the TEE? Using IPC, data is passed through a pointer to the buffer. Memory is reserved 
beforehand for REE / TEE sharing. The TEE is access using ioctl call.
At this moment decryption for PlayReady and Widevine are implemented.

Only video is passed to TEE context, audio stays in REE context.

Boxes are now shipped without PlayReady enabled, as every license has to be paid for. Some countries do not use PlayReady.
For a box in the field, we need to be able to enable PlayReady. This means the device has to receive device credentials.
The device credentials will be passed to the TEE, which will be encrypted using the unique TEE id. The encrypted 
credentials are then sent to the backend for storage. This enables revocation of PlayReady credentials, and the box does 
not need to store the credentials. This helps is servicing and refurbishing of boxes.

PlayReady used a model certificate, which is signed with the device specific key to form the device certificate. 
Normally the model certificate is then removed to disable re-generation of the device certificate by hackers. In the 
metrological approach, the model certificate never is placed on the device, the device certificate is generated on the 
server.

Questions: Can we have control over the presence / absence of the device credentials, by e.g. pushing the device 
credentials when needed, so that we can enable revoking credentials for a device. This means credentials are never 
stored in the device, only temporarily.