
#include <libsal/sal_util.h>

#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <openssl/aes.h>
#include <openssl/md5.h>

static int32 _sal_util_aesKey_gen(EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx)
{
  int i, nrounds = 5;
  int key_data_len = 64;
  unsigned char key[32];
  unsigned char salt[] = "1A3BB2F78D6EC7D8";
  unsigned char iv[32] = "2268BA68768B58C3687D4F205923A741";
  unsigned char key_data[64] = "EC14D4F5BC6B9A3766D31EF9A1BB854121FB938B606462C70B2D0E26549C486A";

  i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);
  if (i != 32) {

      return SYS_ERR_FAILED;
  }

  EVP_CIPHER_CTX_init(e_ctx);
  EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
  EVP_CIPHER_CTX_init(d_ctx);
  EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

  return SYS_ERR_OK;
}

static char *_sal_util_aes_encrypt(EVP_CIPHER_CTX *e, char *plaintext, int *len)
{
  
  int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
  char *ciphertext = malloc(c_len);

  EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);

  EVP_EncryptUpdate(e, (unsigned char *)ciphertext, &c_len, (unsigned char *)plaintext, *len);

  EVP_EncryptFinal_ex(e, (unsigned char *)ciphertext+c_len, &f_len);

  *len = c_len + f_len;
  return ciphertext;
}

static char *_sal_util_aes_decrypt(EVP_CIPHER_CTX *e, char *ciphertext, int *len)
{
  
  int p_len = *len, f_len = 0;
  char *plaintext = malloc(p_len + AES_BLOCK_SIZE);

  EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
  EVP_DecryptUpdate(e, (unsigned char *)plaintext, &p_len, (unsigned char *)ciphertext, *len);
  EVP_DecryptFinal_ex(e, (unsigned char *)plaintext+p_len, &f_len);

  *len = p_len + f_len;

  return plaintext;
}

char *sal_util_base64_encode(char *input, int32 length)
{
    BIO *bmem, *b64;
    BUF_MEM *bptr;
    char *buf = NULL;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    buf = malloc(bptr->length+1);
    osal_memset(buf, 0, bptr->length+1);

    memcpy(buf, bptr->data, bptr->length+1);
    buf[bptr->length] = 0;

    BIO_free_all(b64);

    return buf;
}

char *sal_util_base64_decode(char *input, int32 length)
{
    BIO *b64, *bmem;
    char *buf = NULL;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_new_mem_buf(input, length);
    bmem = BIO_push(b64, bmem);

    buf = malloc(length+1);
    osal_memset(buf, 0, length+1);

    BIO_read(bmem, buf, length+1);

    BIO_free_all(bmem);

    return buf;
}

int32 sal_util_str_encrypt(char *input, char *output)
{
    EVP_CIPHER_CTX en, de;
    char *ciphertext = NULL;
    char *b64 = NULL;
    int32 len = 0;

    if (_sal_util_aesKey_gen(&en, &de))
        return SYS_ERR_FAILED;

    len = osal_strlen(input) + 1;

    ciphertext = _sal_util_aes_encrypt(&en, input, &len);

    if (NULL != ciphertext)
    {
        b64 = sal_util_base64_encode(ciphertext, len);

        if (NULL != b64)
        {
            osal_strcpy(output, b64);

            osal_free(b64);
        }

        osal_free(ciphertext);
    }

    EVP_CIPHER_CTX_cleanup(&en);
    EVP_CIPHER_CTX_cleanup(&de);

    return SYS_ERR_OK;
}

int32 sal_util_str_decrypt(char *input, char *output)
{
    EVP_CIPHER_CTX en, de;
    char *plaintext = NULL;
    char *b64 = NULL;
    int32 len = 0;

    if (_sal_util_aesKey_gen(&en, &de))
        return SYS_ERR_FAILED;

    len = osal_strlen(input);

    b64 = sal_util_base64_decode(input, len);

    if (NULL != b64)
    {
        plaintext = (char *)_sal_util_aes_decrypt(&de, b64, &len);

        if (NULL != plaintext)
        {
            osal_strcpy(output, plaintext);

            osal_free(plaintext);
        }

        osal_free(b64);
    }

    EVP_CIPHER_CTX_cleanup(&en);
    EVP_CIPHER_CTX_cleanup(&de);

    return SYS_ERR_OK;
}

int32 sal_util_md5_hash(char *data, uint32 len, char *md5sum)
{
    uint32 i;
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX md5_ctx;

    MD5_Init(&md5_ctx);

    MD5_Update(&md5_ctx, (unsigned char *)data, len);

    MD5_Final(digest, &md5_ctx);

    for (i = 0; i < MD5_DIGEST_LENGTH; i++)
        osal_sprintf(&md5sum[i * 2], "%02x", digest[i]);

    return SYS_ERR_OK;
}

int32 sal_util_key_generate(sys_crypt_keyType_t type)
{
    FILE *fp = NULL;
    char *tmp = NULL;
    char cmd[SYS_BUF512_LEN];

    SYS_PARAM_CHK((type >= SYS_CRYPT_KEYTYPE_END), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(cmd);

#ifdef CONFIG_SYS_APP_SSH
    if (SYS_CRYPT_KEYTYPE_RSA_V1 == type)
    {
#ifdef CONFIG_USER_SSH_SSHKEYGEN
        BOOTMSG_PRINTF("Generating a SSHv1 default RSA Key.\n");
        BOOTMSG_PRINTF("This may take a few minutes, depending on the key size.\n\n");
        osal_sprintf(cmd, "ssh-keygen -q -t rsa1 -N \"\" -f %s", SYS_SSH_RSAV1_PRIKEY_FILE"_tmp");
        system(cmd);

        if (SYS_ERR_OK == sal_util_file_exist(SYS_SSH_RSAV1_PRIKEY_FILE"_tmp"))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "mv %s %s", SYS_SSH_RSAV1_PRIKEY_FILE"_tmp", SYS_SSH_RSAV1_PRIKEY_FILE);
            system(cmd);
        }
#endif
    }
    else if (SYS_CRYPT_KEYTYPE_RSA_V2 == type)
    {
#ifdef CONFIG_USER_SSH_SSHKEYGEN
        BOOTMSG_PRINTF("Generating a SSHv2 default RSA Key.\n");
        BOOTMSG_PRINTF("This may take a few minutes, depending on the key size.\n\n");
        osal_sprintf(cmd, "ssh-keygen -q -t rsa -N \"\" -f %s", SYS_SSH_RSAV2_PRIKEY_FILE"_tmp");
        system(cmd);

        if (SYS_ERR_OK == sal_util_file_exist(SYS_SSH_RSAV2_PRIKEY_FILE"_tmp"))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "mv %s %s", SYS_SSH_RSAV2_PRIKEY_FILE"_tmp", SYS_SSH_RSAV2_PRIKEY_FILE);
            system(cmd);
        }
#endif
    }
    else if (SYS_CRYPT_KEYTYPE_DSA_V2 == type)
    {
#ifdef CONFIG_USER_SSH_SSHKEYGEN
        BOOTMSG_PRINTF("Generating a SSHv2 default DSA Key.\n");
        BOOTMSG_PRINTF("This may take a few minutes, depending on the key size.\n\n");
        osal_sprintf(cmd, "ssh-keygen -q -t dsa -N \"\" -f %s", SYS_SSH_DSAV2_PRIKEY_FILE"_tmp");
        system(cmd);

        if (SYS_ERR_OK == sal_util_file_exist(SYS_SSH_DSAV2_PRIKEY_FILE"_tmp"))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "mv %s %s", SYS_SSH_DSAV2_PRIKEY_FILE"_tmp", SYS_SSH_DSAV2_PRIKEY_FILE);
            system(cmd);
        }
#endif
    }
    else if (SYS_CRYPT_KEYTYPE_RSA_PUBKEY_V1 == type)
    {
        if (SYS_ERR_OK != sal_util_file_exist(SYS_SSH_RSAV1_PRIKEY_FILE))
            return SYS_ERR_FILE_NOT_EXIST;

        osal_sprintf(cmd, "openssl rsa1 -in %s -pubout > %s", SYS_SSH_RSAV1_PRIKEY_FILE, SYS_SSH_RSAV1_PUBKEY_FILE"_tmp");
        system(cmd);

        if (SYS_ERR_OK == sal_util_file_exist(SYS_SSH_RSAV1_PUBKEY_FILE"_tmp"))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "mv %s %s", SYS_SSH_RSAV1_PUBKEY_FILE"_tmp", SYS_SSH_RSAV1_PUBKEY_FILE);
            system(cmd);
        }
    }
    else if (SYS_CRYPT_KEYTYPE_RSA_PUBKEY_V2 == type)
    {
        if (SYS_ERR_OK != sal_util_file_exist(SYS_SSH_RSAV2_PRIKEY_FILE))
            return SYS_ERR_FILE_NOT_EXIST;

        unlink(SYS_SSH_RSAV2_PUBKEY_FILE);
        osal_sprintf(cmd, "openssl rsa -in %s -pubout > %s", SYS_SSH_RSAV2_PRIKEY_FILE, SYS_SSH_RSAV2_PUBKEY_FILE"_tmp");
        system(cmd);

        if (SYS_ERR_OK == sal_util_file_exist(SYS_SSH_RSAV2_PUBKEY_FILE"_tmp"))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "mv %s %s", SYS_SSH_RSAV2_PUBKEY_FILE"_tmp", SYS_SSH_RSAV2_PUBKEY_FILE);
            system(cmd);
        }
    }
    else if (SYS_CRYPT_KEYTYPE_DSA_PUBKEY_V2 == type)
    {
        if (SYS_ERR_OK != sal_util_file_exist(SYS_SSH_DSAV2_PRIKEY_FILE))
        return SYS_ERR_FILE_NOT_EXIST;

        unlink(SYS_SSH_DSAV2_PUBKEY_FILE);
        osal_sprintf(cmd, "openssl dsa -in %s -pubout > %s", SYS_SSH_DSAV2_PRIKEY_FILE, SYS_SSH_DSAV2_PUBKEY_FILE"_tmp");
        system(cmd);

        if (SYS_ERR_OK == sal_util_file_exist(SYS_SSH_DSAV2_PUBKEY_FILE"_tmp"))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "mv %s %s", SYS_SSH_DSAV2_PUBKEY_FILE"_tmp", SYS_SSH_DSAV2_PUBKEY_FILE);
            system(cmd);
        }
    }
    else
#endif 
    if (SYS_CRYPT_KEYTYPE_SSL_HTTPS == type)
    {
        
        osal_sprintf(cmd, "/bin/openssl req -new -x509 -sha256 -days 365 -nodes -out %s -keyout %s -config %s",
                            SSL_CERT_FILE"_tmp", SSL_CERT_KEY_FILE"_tmp", SYS_SSL_CONFIG_FILE);
        system(cmd);

        if ((SYS_ERR_OK == sal_util_file_exist(SSL_CERT_FILE"_tmp")) &&
            (SYS_ERR_OK == sal_util_file_exist(SSL_CERT_KEY_FILE"_tmp")))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "mv %s %s;mv %s %s", SSL_CERT_FILE"_tmp", SSL_CERT_FILE, SSL_CERT_KEY_FILE"_tmp", SSL_CERT_KEY_FILE);
            system(cmd);
        }
    }
    else if (SYS_CRYPT_KEYTYPE_RSA_HTTP == type)
    {
        osal_sprintf(cmd, "openssl genrsa -out %s 1024 1> /dev/null 2> /dev/null", SYS_SEC_HTTP_RSA_FILE"_tmp");
        system(cmd);

        if (SYS_ERR_OK == sal_util_file_exist(SYS_SEC_HTTP_RSA_FILE"_tmp"))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "openssl rsa -in %s -out %s -noout -modulus 1> /dev/null 2> /dev/null",
                SYS_SEC_HTTP_RSA_FILE"_tmp", SYS_SEC_HTTP_RSA_MODULUS_FILE"_tmp");
            system(cmd);
        }

        if ((SYS_ERR_OK == sal_util_file_exist(SYS_SEC_HTTP_RSA_FILE"_tmp")) &&
            (SYS_ERR_OK == sal_util_file_exist(SYS_SEC_HTTP_RSA_MODULUS_FILE"_tmp")))
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "mv %s %s;mv %s %s", SYS_SEC_HTTP_RSA_FILE"_tmp", SYS_SEC_HTTP_RSA_FILE,
                SYS_SEC_HTTP_RSA_MODULUS_FILE"_tmp", SYS_SEC_HTTP_RSA_MODULUS_FILE);
            system(cmd);
        }
        else
            return SYS_ERR_FAILED;

        if (NULL == (fp = fopen(SYS_SEC_HTTP_RSA_MODULUS_FILE, "r+")))
            return SYS_ERR_FAILED;

        SYS_MEM_CLEAR(cmd);
        if (NULL == fgets(cmd, sizeof(cmd), fp))
        {
            fclose(fp);
            return SYS_ERR_FAILED;
        }

        tmp = index(cmd, '=');
        if (NULL == tmp)
        {
            fclose(fp);
            return SYS_ERR_FAILED;
        }

        fclose(fp);
        if (NULL == (fp = fopen(SYS_SEC_HTTP_RSA_MODULUS_FILE, "w+")))
            return SYS_ERR_FAILED;

        fputs(++tmp, fp);
        fclose(fp);
    }

    return SYS_ERR_OK;
}

int32 sal_util_rsaModulus_get(char *pBuf)
{
    FILE *fp = NULL;

    SYS_PARAM_CHK((NULL == pBuf), SYS_ERR_NULL_POINTER);

    if (NULL == (fp = fopen(SYS_SEC_HTTP_RSA_MODULUS_FILE, "r")))
    {
        
        SYS_ERR_CHK(sal_util_key_generate(SYS_CRYPT_KEYTYPE_RSA_HTTP));

        if (NULL == (fp = fopen(SYS_SEC_HTTP_RSA_MODULUS_FILE, "r")))
            return SYS_ERR_FAILED;
    }

    if (NULL == fgets(pBuf, SYS_BUF512_LEN, fp))
    {
        fclose(fp);
        return SYS_ERR_FAILED;
    }

    fclose(fp);

    pBuf[strlen(pBuf)-1] = 0;

    return SYS_ERR_OK;
}

int32 sal_util_rsa_decrypt(char *privateKey, char *pEcryptMsg, char *pDecryptMsg)
{
    uint32 i = 0;
    FILE *fp = NULL;
    char cmd[SYS_BUF256_LEN];

    SYS_PARAM_CHK((NULL == privateKey), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEcryptMsg), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pDecryptMsg), SYS_ERR_NULL_POINTER);

#define TMP_FILE_1  "/tmp/1"
#define TMP_FILE_2  "/tmp/2"

    if (NULL == (fp = fopen(TMP_FILE_1, "w+")))
        return SYS_ERR_FAILED;

    for (i = 0; i < strlen(pEcryptMsg); i++)
    {
        fputc(*(pEcryptMsg+i), fp);

        if ((i != 0) && (0 == i % 63))
        fputc('\n', fp);
    }

    fclose(fp);

    SYS_MEM_CLEAR(cmd);
    sprintf(cmd, "openssl base64 -d -in %s | openssl rsautl -decrypt -inkey %s > %s",
            TMP_FILE_1, privateKey, TMP_FILE_2);
    system(cmd);

    unlink(TMP_FILE_1);

    if (NULL == (fp = fopen(TMP_FILE_2, "r")))
        return SYS_ERR_FAILED;

    SYS_MEM_CLEAR(cmd);
    if (NULL == fgets(cmd, SYS_BUF256_LEN, fp))
    {
        fclose(fp);
        return SYS_ERR_FAILED;
    }

    fclose(fp);
    unlink(TMP_FILE_2);
    osal_strcpy(pDecryptMsg, cmd);

    return SYS_ERR_OK;
}

