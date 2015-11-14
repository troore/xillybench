
#ifndef __AES_H_
#define __AES_H_

/* key generate */
int KeySchedule (int, int *);
int SubByte (int);

/* encrypto decrypto */
void ByteSub_ShiftRow (int *, int);
void InversShiftRow_ByteSub (int *, int);
int MixColumn_AddRoundKey (int *, int, int);
int AddRoundKey_InversMixColumn (int *, int, int);
int AddRoundKey (int *, int, int);
int encrypt (int *, int *, int);
int decrypt (int *, int *, int);
void decrypt_round1(int *, int);
int aes_main(void);

#endif
