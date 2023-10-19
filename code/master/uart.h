#ifndef UART_H_
#define UART_H_

void uart_init();
void uart_sendbyte(char b);
char uart_recvbyte();


#endif /* UART_H_ */