(type $tinit (func (param)))
(type $tinit_impl (func (param)))
(type $tregister_tensor (func (param i32 i32 i32)))
(type $tregister_tensor_impl (func (param i32 i32 i32)))
(type $tdispose_data (func (param i32)))
(type $tdispose_data_impl (func (param i32)))
(type $tdispose (func (param)))
(type $tdispose_impl (func (param)))
(type $tConv2D (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tConv2D_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tMaxPool (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tMaxPool_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tAvgPool (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tAvgPool_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tDepthwiseConv2dNative (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tDepthwiseConv2dNative_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tResizeBilinear (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tResizeBilinear_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tFusedDepthwiseConv2D (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tFusedDepthwiseConv2D_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tSoftmax (func (param i32 i32 i32 i32)))
(type $tSoftmax_impl (func (param i32 i32 i32 i32)))
(type $tFusedConv2D (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tFusedConv2D_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $t_FusedMatMul (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $t_FusedMatMul_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tClipByValue (func (param i32 f32 f32 i32)))
(type $tClipByValue_impl (func (param i32 f32 f32 i32)))
(type $tPadV2 (func (param i32 i32 i32 i32 i32 i32 f32 i32)))
(type $tPadV2_impl (func (param i32 i32 i32 i32 i32 i32 f32 i32)))
(type $tPrelu (func (param i32 i32 i32)))
(type $tPrelu_impl (func (param i32 i32 i32)))
(type $tSigmoid (func (param i32 i32)))
(type $tSigmoid_impl (func (param i32 i32)))
(type $tCropAndResize (func (param i32 i32 i32 i32 i32 i32 i32 i32 f32 i32)))
(type $tCropAndResize_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32 f32 i32)))
(type $tBatchMatMul (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tBatchMatMul_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tAddN (func (param i32 i32 i32 i32)))
(type $tAddN_impl (func (param i32 i32 i32 i32)))
(type $tNonMaxSuppressionV4 (func (param i32 i32 i32 f32 f32 i32) (result i32)))
(type $tNonMaxSuppressionV4_impl (func (param i32 i32 i32 f32 f32 i32) (result i32)))
(type $tNonMaxSuppressionV5 (func (param i32 i32 i32 f32 f32 f32) (result i32)))
(type $tNonMaxSuppressionV5_impl (func (param i32 i32 i32 f32 f32 f32) (result i32)))
(type $tNonMaxSuppressionV3 (func (param i32 i32 i32 f32 f32) (result i32)))
(type $tNonMaxSuppressionV3_impl (func (param i32 i32 i32 f32 f32) (result i32)))
(type $tMax (func (param i32 i32 i32)))
(type $tMax_impl (func (param i32 i32 i32)))
(type $tSub (func (param i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tSub_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tArgMax (func (param i32 i32 i32 i32 i32)))
(type $tArgMax_impl (func (param i32 i32 i32 i32 i32)))
(type $tFusedBatchNorm (func (param i32 i32 i32 i32 i32 f32 i32)))
(type $tFusedBatchNorm_impl (func (param i32 i32 i32 i32 i32 f32 i32)))
(type $tScatterNd (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tScatterNd_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tConv2DBackpropInput (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tConv2DBackpropInput_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tSelectV2 (func (param i32 i32 i32 i32 i32)))
(type $tSelectV2_impl (func (param i32 i32 i32 i32 i32)))
(type $tDiv (func (param i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tDiv_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tGather (func (param i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tGather_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tGatherNd (func (param i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tGatherNd_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tTile (func (param i32 i32 i32 i32 i32 i32 i32)))
(type $tTile_impl (func (param i32 i32 i32 i32 i32 i32 i32)))
(type $tFlipLeftRight (func (param i32 i32 i32 i32 i32 i32)))
(type $tFlipLeftRight_impl (func (param i32 i32 i32 i32 i32 i32)))
(type $tMin (func (param i32 i32 i32)))
(type $tMin_impl (func (param i32 i32 i32)))
(type $tSum (func (param i32 i32 i32)))
(type $tSum_impl (func (param i32 i32 i32)))
(type $tTranspose (func (param i32 i32 i32 i32 i32 i32 i32)))
(type $tTranspose_impl (func (param i32 i32 i32 i32 i32 i32 i32)))
(type $tOneHot (func (param i32 i32 i32 i32 i32)))
(type $tOneHot_impl (func (param i32 i32 i32 i32 i32)))
(type $tRotateWithOffset (func (param i32 i32 i32 i32 i32 f32 f32 f32 i32 i32 i32)))
(type $tRotateWithOffset_impl (func (param i32 i32 i32 i32 i32 f32 f32 f32 i32 i32 i32)))
(type $tReverse (func (param i32 i32 i32 i32 i32 i32)))
(type $tReverse_impl (func (param i32 i32 i32 i32 i32 i32)))
(type $tMultiply (func (param i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tMultiply_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tAdd (func (param i32 i32 i32 i32 i32 i32 i32 i32)))
(type $tAdd_impl (func (param i32 i32 i32 i32 i32 i32 i32 i32)))
(type $t_Z20__gthrw_pthread_oncePiPFvvE (func (param i32 i32) (result i32)))
(type $t_Z27__gthrw_pthread_getspecificj (func (param i32) (result i32)))
(type $t_Z27__gthrw_pthread_setspecificjPKv (func (param i32 i32) (result i32)))
(type $t_Z22__gthrw_pthread_createPmPK14pthread_attr_tPFPvS3_ES3_ (func (param i32 i32 i32 i32) (result i32)))
(type $t_Z22__gthrw_pthread_cancelm (func (param i32) (result i32)))
(type $t_Z26__gthrw_pthread_mutex_lockP15pthread_mutex_t (func (param i32) (result i32)))
(type $t_Z29__gthrw_pthread_mutex_trylockP15pthread_mutex_t (func (param i32) (result i32)))
(type $t_Z28__gthrw_pthread_mutex_unlockP15pthread_mutex_t (func (param i32) (result i32)))
(type $t_Z26__gthrw_pthread_mutex_initP15pthread_mutex_tPK19pthread_mutexattr_t (func (param i32 i32) (result i32)))
(type $t_Z26__gthrw_pthread_key_createPjPFvPvE (func (param i32 i32) (result i32)))
(type $t_Z26__gthrw_pthread_key_deletej (func (param i32) (result i32)))
(type $t_Z30__gthrw_pthread_mutexattr_initP19pthread_mutexattr_t (func (param i32) (result i32)))
(type $t_Z33__gthrw_pthread_mutexattr_settypeP19pthread_mutexattr_ti (func (param i32 i32) (result i32)))
(type $t_Z33__gthrw_pthread_mutexattr_destroyP19pthread_mutexattr_t (func (param i32) (result i32)))
(func $init (type $tinit)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	call init_impl 
	 return 
)
(func $register_tensor (type $tregister_tensor) (param $r2 i32) (param $r3 i32) (param $r4 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r7 r2 
	i32.mov r8 r3 
	i32.mov r9 r4 
	call register_tensor_impl 
	 return 
)
(func $dispose_data (type $tdispose_data) (param $r2 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r5 r2 
	call dispose_data_impl 
	 return 
)
(func $dispose (type $tdispose)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	call dispose_impl 
	 return 
)
(func $Conv2D (type $tConv2D) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32) (param $r11 i32) (param $r12 i32) (param $r13 i32) (param $r14 i32) (param $r15 i32) (param $r16 i32) (param $r17 i32) (param $r18 i32) (param $r19 i32) (param $r20 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r23 r2 
	i32.mov r24 r3 
	i32.mov r25 r4 
	i32.mov r26 r5 
	i32.mov r27 r6 
	i32.mov r28 r7 
	i32.mov r29 r8 
	i32.mov r30 r9 
	i32.mov r31 r10 
	i32.mov r32 r11 
	i32.mov r33 r12 
	i32.mov r34 r13 
	i32.mov r35 r14 
	i32.mov r36 r15 
	i32.mov r37 r16 
	i32.mov r38 r17 
	i32.mov r39 r18 
	i32.mov r40 r19 
	i32.mov r41 r20 
	call Conv2D_impl 
	 return 
)
(func $MaxPool (type $tMaxPool) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32) (param $r11 i32) (param $r12 i32) (param $r13 i32) (param $r14 i32) (param $r15 i32) (param $r16 i32) (param $r17 i32) (param $r18 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r21 r2 
	i32.mov r22 r3 
	i32.mov r23 r4 
	i32.mov r24 r5 
	i32.mov r25 r6 
	i32.mov r26 r7 
	i32.mov r27 r8 
	i32.mov r28 r9 
	i32.mov r29 r10 
	i32.mov r30 r11 
	i32.mov r31 r12 
	i32.mov r32 r13 
	i32.mov r33 r14 
	i32.mov r34 r15 
	i32.mov r35 r16 
	i32.mov r36 r17 
	i32.mov r37 r18 
	call MaxPool_impl 
	 return 
)
(func $AvgPool (type $tAvgPool) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32) (param $r11 i32) (param $r12 i32) (param $r13 i32) (param $r14 i32) (param $r15 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r18 r2 
	i32.mov r19 r3 
	i32.mov r20 r4 
	i32.mov r21 r5 
	i32.mov r22 r6 
	i32.mov r23 r7 
	i32.mov r24 r8 
	i32.mov r25 r9 
	i32.mov r26 r10 
	i32.mov r27 r11 
	i32.mov r28 r12 
	i32.mov r29 r13 
	i32.mov r30 r14 
	i32.mov r31 r15 
	call AvgPool_impl 
	 return 
)
(func $DepthwiseConv2dNative (type $tDepthwiseConv2dNative) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32) (param $r11 i32) (param $r12 i32) (param $r13 i32) (param $r14 i32) (param $r15 i32) (param $r16 i32) (param $r17 i32) (param $r18 i32) (param $r19 i32) (param $r20 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r23 r2 
	i32.mov r24 r3 
	i32.mov r25 r4 
	i32.mov r26 r5 
	i32.mov r27 r6 
	i32.mov r28 r7 
	i32.mov r29 r8 
	i32.mov r30 r9 
	i32.mov r31 r10 
	i32.mov r32 r11 
	i32.mov r33 r12 
	i32.mov r34 r13 
	i32.mov r35 r14 
	i32.mov r36 r15 
	i32.mov r37 r16 
	i32.mov r38 r17 
	i32.mov r39 r18 
	i32.mov r40 r19 
	i32.mov r41 r20 
	call DepthwiseConv2dNative_impl 
	 return 
)
(func $ResizeBilinear (type $tResizeBilinear) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32)
    (local $r11 i32) # in-out reg start index [r:12 rd: 0 f: 0 fd: 0]
	i32.const r11 255 
	i32.and r9 r9 r11 
	i32.mov r14 r2 
	i32.mov r15 r3 
	i32.mov r16 r4 
	i32.mov r17 r5 
	i32.mov r18 r6 
	i32.mov r19 r7 
	i32.mov r20 r8 
	i32.mov r21 r9 
	i32.mov r22 r10 
	call ResizeBilinear_impl 
	 return 
)
(func $FusedDepthwiseConv2D (type $tFusedDepthwiseConv2D) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32) (param $r11 i32) (param $r12 i32) (param $r13 i32) (param $r14 i32) (param $r15 i32) (param $r16 i32) (param $r17 i32) (param $r18 i32) (param $r19 i32) (param $r20 i32) (param $r21 i32) (param $r22 i32) (param $r23 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r26 r2 
	i32.mov r27 r3 
	i32.mov r28 r4 
	i32.mov r29 r5 
	i32.mov r30 r6 
	i32.mov r31 r7 
	i32.mov r32 r8 
	i32.mov r33 r9 
	i32.mov r34 r10 
	i32.mov r35 r11 
	i32.mov r36 r12 
	i32.mov r37 r13 
	i32.mov r38 r14 
	i32.mov r39 r15 
	i32.mov r40 r16 
	i32.mov r41 r17 
	i32.mov r42 r18 
	i32.mov r43 r19 
	i32.mov r44 r20 
	i32.mov r45 r21 
	i32.mov r46 r22 
	i32.mov r47 r23 
	call FusedDepthwiseConv2D_impl 
	 return 
)
(func $Softmax (type $tSoftmax) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r8 r2 
	i32.mov r9 r3 
	i32.mov r10 r4 
	i32.mov r11 r5 
	call Softmax_impl 
	 return 
)
(func $FusedConv2D (type $tFusedConv2D) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32) (param $r11 i32) (param $r12 i32) (param $r13 i32) (param $r14 i32) (param $r15 i32) (param $r16 i32) (param $r17 i32) (param $r18 i32) (param $r19 i32) (param $r20 i32) (param $r21 i32) (param $r22 i32) (param $r23 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r26 r2 
	i32.mov r27 r3 
	i32.mov r28 r4 
	i32.mov r29 r5 
	i32.mov r30 r6 
	i32.mov r31 r7 
	i32.mov r32 r8 
	i32.mov r33 r9 
	i32.mov r34 r10 
	i32.mov r35 r11 
	i32.mov r36 r12 
	i32.mov r37 r13 
	i32.mov r38 r14 
	i32.mov r39 r15 
	i32.mov r40 r16 
	i32.mov r41 r17 
	i32.mov r42 r18 
	i32.mov r43 r19 
	i32.mov r44 r20 
	i32.mov r45 r21 
	i32.mov r46 r22 
	i32.mov r47 r23 
	call FusedConv2D_impl 
	 return 
)
(func $_FusedMatMul (type $t_FusedMatMul) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32) (param $r11 i32) (param $r12 i32) (param $r13 i32)
    (local $r14 i32) (local $r15 i32) # in-out reg start index [r:16 rd: 0 f: 0 fd: 0]
	i32.const r14 255 
	i32.and r8 r8 r14 
	i32.const r15 255 
	i32.and r9 r9 r15 
	i32.mov r18 r2 
	i32.mov r19 r3 
	i32.mov r20 r4 
	i32.mov r21 r5 
	i32.mov r22 r6 
	i32.mov r23 r7 
	i32.mov r24 r8 
	i32.mov r25 r9 
	i32.mov r26 r10 
	i32.mov r27 r11 
	i32.mov r28 r12 
	i32.mov r29 r13 
	call _FusedMatMul_impl 
	 return 
)
(func $ClipByValue (type $tClipByValue) (param $r2 i32) (param $f2 f32) (param $f3 f32) (param $r3 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r6 r2 
	f32.mov f6 f2 
	f32.mov f7 f3 
	i32.mov r7 r3 
	call ClipByValue_impl 
	 return 
)
(func $PadV2 (type $tPadV2) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $f2 f32) (param $r8 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r11 r2 
	i32.mov r12 r3 
	i32.mov r13 r4 
	i32.mov r14 r5 
	i32.mov r15 r6 
	i32.mov r16 r7 
	f32.mov f5 f2 
	i32.mov r17 r8 
	call PadV2_impl 
	 return 
)
(func $Prelu (type $tPrelu) (param $r2 i32) (param $r3 i32) (param $r4 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r7 r2 
	i32.mov r8 r3 
	i32.mov r9 r4 
	call Prelu_impl 
	 return 
)
(func $Sigmoid (type $tSigmoid) (param $r2 i32) (param $r3 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r6 r2 
	i32.mov r7 r3 
	call Sigmoid_impl 
	 return 
)
(func $CropAndResize (type $tCropAndResize) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $f2 f32) (param $r10 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r13 r2 
	i32.mov r14 r3 
	i32.mov r15 r4 
	i32.mov r16 r5 
	i32.mov r17 r6 
	i32.mov r18 r7 
	i32.mov r19 r8 
	i32.mov r20 r9 
	f32.mov f5 f2 
	i32.mov r21 r10 
	call CropAndResize_impl 
	 return 
)
(func $BatchMatMul (type $tBatchMatMul) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32)
    (local $r11 i32) (local $r12 i32) # in-out reg start index [r:13 rd: 0 f: 0 fd: 0]
	i32.const r11 255 
	i32.and r8 r8 r11 
	i32.const r12 255 
	i32.and r9 r9 r12 
	i32.mov r15 r2 
	i32.mov r16 r3 
	i32.mov r17 r4 
	i32.mov r18 r5 
	i32.mov r19 r6 
	i32.mov r20 r7 
	i32.mov r21 r8 
	i32.mov r22 r9 
	i32.mov r23 r10 
	call BatchMatMul_impl 
	 return 
)
(func $AddN (type $tAddN) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r8 r2 
	i32.mov r9 r3 
	i32.mov r10 r4 
	i32.mov r11 r5 
	call AddN_impl 
	 return 
)
(func $NonMaxSuppressionV4 (type $tNonMaxSuppressionV4) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $f2 f32) (param $f3 f32) (param $r5 i32) (result i32)
    (local $r6 i32) (local $r7 i32) # in-out reg start index [r:8 rd: 0 f: 0 fd: 0]
	i32.const r6 255 
	i32.and r5 r5 r6 
	i32.mov r10 r2 
	i32.mov r11 r3 
	i32.mov r12 r4 
	f32.mov f6 f2 
	f32.mov f7 f3 
	i32.mov r13 r5 
	call NonMaxSuppressionV4_impl 
	i32.mov r7 r8 
	i32.mov r0 r7 
	 return 
)
(func $NonMaxSuppressionV5 (type $tNonMaxSuppressionV5) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $f2 f32) (param $f3 f32) (param $f4 f32) (result i32)
    (local $r5 i32) # in-out reg start index [r:6 rd: 0 f: 0 fd: 0]
	i32.mov r8 r2 
	i32.mov r9 r3 
	i32.mov r10 r4 
	f32.mov f7 f2 
	f32.mov f8 f3 
	f32.mov f9 f4 
	call NonMaxSuppressionV5_impl 
	i32.mov r5 r6 
	i32.mov r0 r5 
	 return 
)
(func $NonMaxSuppressionV3 (type $tNonMaxSuppressionV3) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $f2 f32) (param $f3 f32) (result i32)
    (local $r5 i32) # in-out reg start index [r:6 rd: 0 f: 0 fd: 0]
	i32.mov r8 r2 
	i32.mov r9 r3 
	i32.mov r10 r4 
	f32.mov f6 f2 
	f32.mov f7 f3 
	call NonMaxSuppressionV3_impl 
	i32.mov r5 r6 
	i32.mov r0 r5 
	 return 
)
(func $Max (type $tMax) (param $r2 i32) (param $r3 i32) (param $r4 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r7 r2 
	i32.mov r8 r3 
	i32.mov r9 r4 
	call Max_impl 
	 return 
)
(func $Sub (type $tSub) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r12 r2 
	i32.mov r13 r3 
	i32.mov r14 r4 
	i32.mov r15 r5 
	i32.mov r16 r6 
	i32.mov r17 r7 
	i32.mov r18 r8 
	i32.mov r19 r9 
	call Sub_impl 
	 return 
)
(func $ArgMax (type $tArgMax) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r9 r2 
	i32.mov r10 r3 
	i32.mov r11 r4 
	i32.mov r12 r5 
	i32.mov r13 r6 
	call ArgMax_impl 
	 return 
)
(func $FusedBatchNorm (type $tFusedBatchNorm) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $f2 f32) (param $r7 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r10 r2 
	i32.mov r11 r3 
	i32.mov r12 r4 
	i32.mov r13 r5 
	i32.mov r14 r6 
	f32.mov f5 f2 
	i32.mov r15 r7 
	call FusedBatchNorm_impl 
	 return 
)
(func $ScatterNd (type $tScatterNd) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r13 r2 
	i32.mov r14 r3 
	i32.mov r15 r4 
	i32.mov r16 r5 
	i32.mov r17 r6 
	i32.mov r18 r7 
	i32.mov r19 r8 
	i32.mov r20 r9 
	i32.mov r21 r10 
	call ScatterNd_impl 
	 return 
)
(func $Conv2DBackpropInput (type $tConv2DBackpropInput) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32) (param $r10 i32) (param $r11 i32) (param $r12 i32) (param $r13 i32) (param $r14 i32) (param $r15 i32) (param $r16 i32) (param $r17 i32) (param $r18 i32) (param $r19 i32) (param $r20 i32) (param $r21 i32) (param $r22 i32) (param $r23 i32) (param $r24 i32) (param $r25 i32) (param $r26 i32) (param $r27 i32) (param $r28 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r31 r2 
	i32.mov r32 r3 
	i32.mov r33 r4 
	i32.mov r34 r5 
	i32.mov r35 r6 
	i32.mov r36 r7 
	i32.mov r37 r8 
	i32.mov r38 r9 
	i32.mov r39 r10 
	i32.mov r40 r11 
	i32.mov r41 r12 
	i32.mov r42 r13 
	i32.mov r43 r14 
	i32.mov r44 r15 
	i32.mov r45 r16 
	i32.mov r46 r17 
	i32.mov r47 r18 
	i32.mov r48 r19 
	i32.mov r49 r20 
	i32.mov r50 r21 
	i32.mov r51 r22 
	i32.mov r52 r23 
	i32.mov r53 r24 
	i32.mov r54 r25 
	i32.mov r55 r26 
	i32.mov r56 r27 
	i32.mov r57 r28 
	call Conv2DBackpropInput_impl 
	 return 
)
(func $SelectV2 (type $tSelectV2) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r9 r2 
	i32.mov r10 r3 
	i32.mov r11 r4 
	i32.mov r12 r5 
	i32.mov r13 r6 
	call SelectV2_impl 
	 return 
)
(func $Div (type $tDiv) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r12 r2 
	i32.mov r13 r3 
	i32.mov r14 r4 
	i32.mov r15 r5 
	i32.mov r16 r6 
	i32.mov r17 r7 
	i32.mov r18 r8 
	i32.mov r19 r9 
	call Div_impl 
	 return 
)
(func $Gather (type $tGather) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r12 r2 
	i32.mov r13 r3 
	i32.mov r14 r4 
	i32.mov r15 r5 
	i32.mov r16 r6 
	i32.mov r17 r7 
	i32.mov r18 r8 
	i32.mov r19 r9 
	call Gather_impl 
	 return 
)
(func $GatherNd (type $tGatherNd) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r12 r2 
	i32.mov r13 r3 
	i32.mov r14 r4 
	i32.mov r15 r5 
	i32.mov r16 r6 
	i32.mov r17 r7 
	i32.mov r18 r8 
	i32.mov r19 r9 
	call GatherNd_impl 
	 return 
)
(func $Tile (type $tTile) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r11 r2 
	i32.mov r12 r3 
	i32.mov r13 r4 
	i32.mov r14 r5 
	i32.mov r15 r6 
	i32.mov r16 r7 
	i32.mov r17 r8 
	call Tile_impl 
	 return 
)
(func $FlipLeftRight (type $tFlipLeftRight) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r10 r2 
	i32.mov r11 r3 
	i32.mov r12 r4 
	i32.mov r13 r5 
	i32.mov r14 r6 
	i32.mov r15 r7 
	call FlipLeftRight_impl 
	 return 
)
(func $Min (type $tMin) (param $r2 i32) (param $r3 i32) (param $r4 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r7 r2 
	i32.mov r8 r3 
	i32.mov r9 r4 
	call Min_impl 
	 return 
)
(func $Sum (type $tSum) (param $r2 i32) (param $r3 i32) (param $r4 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r7 r2 
	i32.mov r8 r3 
	i32.mov r9 r4 
	call Sum_impl 
	 return 
)
(func $Transpose (type $tTranspose) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r11 r2 
	i32.mov r12 r3 
	i32.mov r13 r4 
	i32.mov r14 r5 
	i32.mov r15 r6 
	i32.mov r16 r7 
	i32.mov r17 r8 
	call Transpose_impl 
	 return 
)
(func $OneHot (type $tOneHot) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r9 r2 
	i32.mov r10 r3 
	i32.mov r11 r4 
	i32.mov r12 r5 
	i32.mov r13 r6 
	call OneHot_impl 
	 return 
)
(func $RotateWithOffset (type $tRotateWithOffset) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $f2 f32) (param $f3 f32) (param $f4 f32) (param $r7 i32) (param $r8 i32) (param $r9 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r12 r2 
	i32.mov r13 r3 
	i32.mov r14 r4 
	i32.mov r15 r5 
	i32.mov r16 r6 
	f32.mov f7 f2 
	f32.mov f8 f3 
	f32.mov f9 f4 
	i32.mov r17 r7 
	i32.mov r18 r8 
	i32.mov r19 r9 
	call RotateWithOffset_impl 
	 return 
)
(func $Reverse (type $tReverse) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r10 r2 
	i32.mov r11 r3 
	i32.mov r12 r4 
	i32.mov r13 r5 
	i32.mov r14 r6 
	i32.mov r15 r7 
	call Reverse_impl 
	 return 
)
(func $Multiply (type $tMultiply) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r12 r2 
	i32.mov r13 r3 
	i32.mov r14 r4 
	i32.mov r15 r5 
	i32.mov r16 r6 
	i32.mov r17 r7 
	i32.mov r18 r8 
	i32.mov r19 r9 
	call Multiply_impl 
	 return 
)
(func $Add (type $tAdd) (param $r2 i32) (param $r3 i32) (param $r4 i32) (param $r5 i32) (param $r6 i32) (param $r7 i32) (param $r8 i32) (param $r9 i32)
    # in-out reg start index [r:0 rd: 0 f: 0 fd: 0]
	i32.mov r12 r2 
	i32.mov r13 r3 
	i32.mov r14 r4 
	i32.mov r15 r5 
	i32.mov r16 r6 
	i32.mov r17 r7 
	i32.mov r18 r8 
	i32.mov r19 r9 
	call Add_impl 
	 return 
)

