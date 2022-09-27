(type $tadd (func (param i32 i32) (result i32)))
(func $add (type $tadd) (param $r2 i32) (param $r3 i32) (result i32)
    (local $r4 i32) # in-out reg start index [r:5 rd: 0 f: 0 fd: 0]
	i32.add r4 r2 r3 
	i32.mov r0 r4 
	 return 
)

