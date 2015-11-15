#ifndef __MARKER_H_
#define __MARKER_H_

int read_byte (void);
short read_word (void);
int first_marker (void);
int next_marker (void);
void get_sof ();
void get_sos ();
void get_dht ();
void get_dqt ();
void read_markers (unsigned char *buf);

#endif
