from Crypto.Cipher import AES

def aes_128_ecb(key, input):
  cipher = AES.new(key, AES.MODE_ECB)
  return cipher.encrypt(input)

input = b'\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF'
key = b'\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F'

output = aes_128_ecb(key, input)
print(output)
