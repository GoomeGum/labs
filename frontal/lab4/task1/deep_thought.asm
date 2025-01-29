
deep_thought:     file format elf32-i386


Disassembly of section .init:

080482d0 <_init>:
 80482d0:	53                   	push   %ebx
 80482d1:	83 ec 08             	sub    $0x8,%esp
 80482d4:	e8 a7 00 00 00       	call   8048380 <__x86.get_pc_thunk.bx>
 80482d9:	81 c3 27 1d 00 00    	add    $0x1d27,%ebx
 80482df:	8b 83 fc ff ff ff    	mov    -0x4(%ebx),%eax
 80482e5:	85 c0                	test   %eax,%eax
 80482e7:	74 05                	je     80482ee <_init+0x1e>
 80482e9:	e8 42 00 00 00       	call   8048330 <__gmon_start__@plt>
 80482ee:	83 c4 08             	add    $0x8,%esp
 80482f1:	5b                   	pop    %ebx
 80482f2:	c3                   	ret

Disassembly of section .plt:

08048300 <.plt>:
 8048300:	ff 35 04 a0 04 08    	push   0x804a004
 8048306:	ff 25 08 a0 04 08    	jmp    *0x804a008
 804830c:	00 00                	add    %al,(%eax)
	...

08048310 <sleep@plt>:
 8048310:	ff 25 0c a0 04 08    	jmp    *0x804a00c
 8048316:	68 00 00 00 00       	push   $0x0
 804831b:	e9 e0 ff ff ff       	jmp    8048300 <.plt>

08048320 <puts@plt>:
 8048320:	ff 25 10 a0 04 08    	jmp    *0x804a010
 8048326:	68 08 00 00 00       	push   $0x8
 804832b:	e9 d0 ff ff ff       	jmp    8048300 <.plt>

08048330 <__gmon_start__@plt>:
 8048330:	ff 25 14 a0 04 08    	jmp    *0x804a014
 8048336:	68 10 00 00 00       	push   $0x10
 804833b:	e9 c0 ff ff ff       	jmp    8048300 <.plt>

08048340 <__libc_start_main@plt>:
 8048340:	ff 25 18 a0 04 08    	jmp    *0x804a018
 8048346:	68 18 00 00 00       	push   $0x18
 804834b:	e9 b0 ff ff ff       	jmp    8048300 <.plt>

Disassembly of section .text:

08048350 <_start>:
 8048350:	31 ed                	xor    %ebp,%ebp
 8048352:	5e                   	pop    %esi
 8048353:	89 e1                	mov    %esp,%ecx
 8048355:	83 e4 f0             	and    $0xfffffff0,%esp
 8048358:	50                   	push   %eax
 8048359:	54                   	push   %esp
 804835a:	52                   	push   %edx
 804835b:	68 00 85 04 08       	push   $0x8048500
 8048360:	68 90 84 04 08       	push   $0x8048490
 8048365:	51                   	push   %ecx
 8048366:	56                   	push   %esi
 8048367:	68 4d 84 04 08       	push   $0x804844d
 804836c:	e8 cf ff ff ff       	call   8048340 <__libc_start_main@plt>
 8048371:	f4                   	hlt
 8048372:	66 90                	xchg   %ax,%ax
 8048374:	66 90                	xchg   %ax,%ax
 8048376:	66 90                	xchg   %ax,%ax
 8048378:	66 90                	xchg   %ax,%ax
 804837a:	66 90                	xchg   %ax,%ax
 804837c:	66 90                	xchg   %ax,%ax
 804837e:	66 90                	xchg   %ax,%ax

08048380 <__x86.get_pc_thunk.bx>:
 8048380:	8b 1c 24             	mov    (%esp),%ebx
 8048383:	c3                   	ret
 8048384:	66 90                	xchg   %ax,%ax
 8048386:	66 90                	xchg   %ax,%ax
 8048388:	66 90                	xchg   %ax,%ax
 804838a:	66 90                	xchg   %ax,%ax
 804838c:	66 90                	xchg   %ax,%ax
 804838e:	66 90                	xchg   %ax,%ax

08048390 <deregister_tm_clones>:
 8048390:	b8 27 a0 04 08       	mov    $0x804a027,%eax
 8048395:	2d 24 a0 04 08       	sub    $0x804a024,%eax
 804839a:	83 f8 06             	cmp    $0x6,%eax
 804839d:	77 01                	ja     80483a0 <deregister_tm_clones+0x10>
 804839f:	c3                   	ret
 80483a0:	b8 00 00 00 00       	mov    $0x0,%eax
 80483a5:	85 c0                	test   %eax,%eax
 80483a7:	74 f6                	je     804839f <deregister_tm_clones+0xf>
 80483a9:	55                   	push   %ebp
 80483aa:	89 e5                	mov    %esp,%ebp
 80483ac:	83 ec 18             	sub    $0x18,%esp
 80483af:	c7 04 24 24 a0 04 08 	movl   $0x804a024,(%esp)
 80483b6:	ff d0                	call   *%eax
 80483b8:	c9                   	leave
 80483b9:	c3                   	ret
 80483ba:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

080483c0 <register_tm_clones>:
 80483c0:	b8 24 a0 04 08       	mov    $0x804a024,%eax
 80483c5:	2d 24 a0 04 08       	sub    $0x804a024,%eax
 80483ca:	c1 f8 02             	sar    $0x2,%eax
 80483cd:	89 c2                	mov    %eax,%edx
 80483cf:	c1 ea 1f             	shr    $0x1f,%edx
 80483d2:	01 d0                	add    %edx,%eax
 80483d4:	d1 f8                	sar    $1,%eax
 80483d6:	75 01                	jne    80483d9 <register_tm_clones+0x19>
 80483d8:	c3                   	ret
 80483d9:	ba 00 00 00 00       	mov    $0x0,%edx
 80483de:	85 d2                	test   %edx,%edx
 80483e0:	74 f6                	je     80483d8 <register_tm_clones+0x18>
 80483e2:	55                   	push   %ebp
 80483e3:	89 e5                	mov    %esp,%ebp
 80483e5:	83 ec 18             	sub    $0x18,%esp
 80483e8:	89 44 24 04          	mov    %eax,0x4(%esp)
 80483ec:	c7 04 24 24 a0 04 08 	movl   $0x804a024,(%esp)
 80483f3:	ff d2                	call   *%edx
 80483f5:	c9                   	leave
 80483f6:	c3                   	ret
 80483f7:	89 f6                	mov    %esi,%esi
 80483f9:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

08048400 <__do_global_dtors_aux>:
 8048400:	80 3d 24 a0 04 08 00 	cmpb   $0x0,0x804a024
 8048407:	75 13                	jne    804841c <__do_global_dtors_aux+0x1c>
 8048409:	55                   	push   %ebp
 804840a:	89 e5                	mov    %esp,%ebp
 804840c:	83 ec 08             	sub    $0x8,%esp
 804840f:	e8 7c ff ff ff       	call   8048390 <deregister_tm_clones>
 8048414:	c6 05 24 a0 04 08 01 	movb   $0x1,0x804a024
 804841b:	c9                   	leave
 804841c:	f3 c3                	repz ret
 804841e:	66 90                	xchg   %ax,%ax

08048420 <frame_dummy>:
 8048420:	a1 10 9f 04 08       	mov    0x8049f10,%eax
 8048425:	85 c0                	test   %eax,%eax
 8048427:	74 1f                	je     8048448 <frame_dummy+0x28>
 8048429:	b8 00 00 00 00       	mov    $0x0,%eax
 804842e:	85 c0                	test   %eax,%eax
 8048430:	74 16                	je     8048448 <frame_dummy+0x28>
 8048432:	55                   	push   %ebp
 8048433:	89 e5                	mov    %esp,%ebp
 8048435:	83 ec 18             	sub    $0x18,%esp
 8048438:	c7 04 24 10 9f 04 08 	movl   $0x8049f10,(%esp)
 804843f:	ff d0                	call   *%eax
 8048441:	c9                   	leave
 8048442:	e9 79 ff ff ff       	jmp    80483c0 <register_tm_clones>
 8048447:	90                   	nop
 8048448:	e9 73 ff ff ff       	jmp    80483c0 <register_tm_clones>

0804844d <main>:
 804844d:	55                   	push   %ebp
 804844e:	89 e5                	mov    %esp,%ebp
 8048450:	83 e4 f0             	and    $0xfffffff0,%esp
 8048453:	83 ec 10             	sub    $0x10,%esp
 8048456:	c7 04 24 20 85 04 08 	movl   $0x8048520,(%esp)
 804845d:	e8 be fe ff ff       	call   8048320 <puts@plt>
 8048462:	c9                   	leave
 8048463:	c3                   	ret

08048464 <hacked>:
 8048464:	55                   	push   %ebp
 8048465:	89 e5                	mov    %esp,%ebp
 8048467:	83 ec 18             	sub    $0x18,%esp
 804846a:	c7 04 24 4c 85 04 08 	movl   $0x804854c,(%esp)
 8048471:	e8 aa fe ff ff       	call   8048320 <puts@plt>
 8048476:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
 804847d:	e8 8e fe ff ff       	call   8048310 <sleep@plt>
 8048482:	eb e6                	jmp    804846a <hacked+0x6>
 8048484:	66 90                	xchg   %ax,%ax
 8048486:	66 90                	xchg   %ax,%ax
 8048488:	66 90                	xchg   %ax,%ax
 804848a:	66 90                	xchg   %ax,%ax
 804848c:	66 90                	xchg   %ax,%ax
 804848e:	66 90                	xchg   %ax,%ax

08048490 <__libc_csu_init>:
 8048490:	55                   	push   %ebp
 8048491:	57                   	push   %edi
 8048492:	31 ff                	xor    %edi,%edi
 8048494:	56                   	push   %esi
 8048495:	53                   	push   %ebx
 8048496:	e8 e5 fe ff ff       	call   8048380 <__x86.get_pc_thunk.bx>
 804849b:	81 c3 65 1b 00 00    	add    $0x1b65,%ebx
 80484a1:	83 ec 1c             	sub    $0x1c,%esp
 80484a4:	8b 6c 24 30          	mov    0x30(%esp),%ebp
 80484a8:	8d b3 0c ff ff ff    	lea    -0xf4(%ebx),%esi
 80484ae:	e8 1d fe ff ff       	call   80482d0 <_init>
 80484b3:	8d 83 08 ff ff ff    	lea    -0xf8(%ebx),%eax
 80484b9:	29 c6                	sub    %eax,%esi
 80484bb:	c1 fe 02             	sar    $0x2,%esi
 80484be:	85 f6                	test   %esi,%esi
 80484c0:	74 27                	je     80484e9 <__libc_csu_init+0x59>
 80484c2:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 80484c8:	8b 44 24 38          	mov    0x38(%esp),%eax
 80484cc:	89 2c 24             	mov    %ebp,(%esp)
 80484cf:	89 44 24 08          	mov    %eax,0x8(%esp)
 80484d3:	8b 44 24 34          	mov    0x34(%esp),%eax
 80484d7:	89 44 24 04          	mov    %eax,0x4(%esp)
 80484db:	ff 94 bb 08 ff ff ff 	call   *-0xf8(%ebx,%edi,4)
 80484e2:	83 c7 01             	add    $0x1,%edi
 80484e5:	39 f7                	cmp    %esi,%edi
 80484e7:	75 df                	jne    80484c8 <__libc_csu_init+0x38>
 80484e9:	83 c4 1c             	add    $0x1c,%esp
 80484ec:	5b                   	pop    %ebx
 80484ed:	5e                   	pop    %esi
 80484ee:	5f                   	pop    %edi
 80484ef:	5d                   	pop    %ebp
 80484f0:	c3                   	ret
 80484f1:	eb 0d                	jmp    8048500 <__libc_csu_fini>
 80484f3:	90                   	nop
 80484f4:	90                   	nop
 80484f5:	90                   	nop
 80484f6:	90                   	nop
 80484f7:	90                   	nop
 80484f8:	90                   	nop
 80484f9:	90                   	nop
 80484fa:	90                   	nop
 80484fb:	90                   	nop
 80484fc:	90                   	nop
 80484fd:	90                   	nop
 80484fe:	90                   	nop
 80484ff:	90                   	nop

08048500 <__libc_csu_fini>:
 8048500:	f3 c3                	repz ret

Disassembly of section .fini:

08048504 <_fini>:
 8048504:	53                   	push   %ebx
 8048505:	83 ec 08             	sub    $0x8,%esp
 8048508:	e8 73 fe ff ff       	call   8048380 <__x86.get_pc_thunk.bx>
 804850d:	81 c3 f3 1a 00 00    	add    $0x1af3,%ebx
 8048513:	83 c4 08             	add    $0x8,%esp
 8048516:	5b                   	pop    %ebx
 8048517:	c3                   	ret
