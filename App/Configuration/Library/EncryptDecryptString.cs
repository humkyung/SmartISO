using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace Configuration.Library
{
    public class EncryptDecryptQueryString
    {
        private byte[] key = { };
        private byte[] IV = { 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef };

        public string Decrypt(string p_toDecrypt)
        {
            return Decrypt(p_toDecrypt, "r0b1nr0y");
        }

        public string Decrypt(string p_toDecrypt, string p_encryptionKey)
        {
            var inputByteArray = new byte[p_toDecrypt.Length + 1];
            try
            {
                this.key = Encoding.UTF8.GetBytes(p_encryptionKey);
                var des = new DESCryptoServiceProvider();
                inputByteArray = Convert.FromBase64String(p_toDecrypt);
                var ms = new MemoryStream();
                var cs = new CryptoStream(ms, des.CreateDecryptor(this.key, this.IV), CryptoStreamMode.Write);
                cs.Write(inputByteArray, 0, inputByteArray.Length);
                cs.FlushFinalBlock();
                var encoding = Encoding.UTF8;
                return encoding.GetString(ms.ToArray());
            }
            catch (Exception e)
            {
                return e.Message;
            }
        }

        public string Encrypt(string p_toEncrypt)
        {
            return Encrypt(p_toEncrypt, "r0b1nr0y");
        }

        public string Encrypt(string p_toEncrypt, string p_encryptionKey)
        {
            try
            {
                this.key = Encoding.UTF8.GetBytes(p_encryptionKey);
                var des = new DESCryptoServiceProvider();
                var inputByteArray = Encoding.UTF8.GetBytes(p_toEncrypt);
                var ms = new MemoryStream();
                var cs = new CryptoStream(ms, des.CreateEncryptor(this.key, this.IV), CryptoStreamMode.Write);
                cs.Write(inputByteArray, 0, inputByteArray.Length);
                cs.FlushFinalBlock();
                return Convert.ToBase64String(ms.ToArray());
            }
            catch (Exception e)
            {
                return e.Message;
            }
        }
    }
}
