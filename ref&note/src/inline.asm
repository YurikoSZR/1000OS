0000000000001149 <main>:
    1149:	f3 0f 1e fa          	endbr64 
    114d:	55                   	push   %rbp
    114e:	48 89 e5             	mov    %rsp,%rbp
    1151:	48 83 ec 10          	sub    $0x10,%rsp
    1155:	c7 45 f8 05 00 00 00 	movl   $0x5,-0x8(%rbp)
    115c:	c7 45 fc 05 00 00 00 	movl   $0x5,-0x4(%rbp)
    1163:	8b 45 fc             	mov    -0x4(%rbp),%eax
    1166:	0f af c0             	imul   %eax,%eax
    1169:	89 45 f8             	mov    %eax,-0x8(%rbp)
    116c:	8b 45 f8             	mov    -0x8(%rbp),%eax
    116f:	89 c6                	mov    %eax,%esi
    1171:	48 8d 05 8c 0e 00 00 	lea    0xe8c(%rip),%rax        # 2004 <_IO_stdin_used+0x4>
    1178:	48 89 c7             	mov    %rax,%rdi
    117b:	b8 00 00 00 00       	mov    $0x0,%eax
    1180:	e8 cb fe ff ff       	call   1050 <printf@plt>
    1185:	b8 00 00 00 00       	mov    $0x0,%eax
    118a:	c9                   	leave  
    118b:	c3                   	ret    

 
