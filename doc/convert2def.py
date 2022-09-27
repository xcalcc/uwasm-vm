#!/usr/bin/python3

#  
# Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
#  

import pandas as pd
import sys
import os

cur_dir_name = os.path.dirname(os.path.realpath(sys.argv[0]))
proj_dir_name = os.path.dirname(cur_dir_name)

csv_file = 'u_opcode_list.csv'
opcode_list_f = os.path.join(proj_dir_name, 'src/ubh/u_opcode_list.def')
func_list_f = os.path.join(proj_dir_name, 'src/interpreter/u_func_list.def')
opcode_list_l_f = os.path.join(proj_dir_name, 'src/ubh/u_opcode_list_l.def')
impl_f = os.path.join(proj_dir_name, 'src/interpreter/exec_unit_func.h')
case_f = os.path.join(proj_dir_name, 'src/interpreter/exec_unit_op.def')
diff_type_f = os.path.join(proj_dir_name, 'src/ubh/u_diff_type.def')

valtype_list = ['i32', 'i64', 'f32', 'f64']
valtype_num = len(valtype_list)

csv_data = pd.read_csv(csv_file)


def write_file(file, arr):
    with open(file, 'w') as f:
        f.writelines(arr)


def write_op_list():
    """
    write opcode list(8 bit version)
    :return:
    """
    op_list = csv_data.apply(lambda x: '// {}: unused\n'.format(x['opcode_l'])
                             if pd.isna(x['ty_am'])
                             else 'UOPCODE( {}, {}, "{}", {} )\n'.format(x['opcode_l'], x['op_enum'], x['name'], x['ty_am']),
                             axis=1)
    write_file(opcode_list_f, op_list.tolist())


def gen_diff_type_entry(opcode_name, row, valty):
    t1 = row['t1']
    t2 = row['t2']
    t3 = row['t3']
    final_t1 = 'P_NONE'
    final_t2 = 'P_NONE'
    final_t3 = 'P_NONE'
    exist = row.notna()
    if exist['t1']:
        final_t1 = ('P_%s' % valty) if t1 == 'TY_VAL' else t1
    if exist['t2']:
        final_t2 = ('P_%s' % valty) if t2 == 'TY_VAL' else t2
    if exist['t3']:
        final_t3 = ('P_%s' % valty) if t3 == 'TY_VAL' else t3
    return 'U_DIFF_TYPE(%s, %s, %s, %s)\n' % (opcode_name, final_t1, final_t2, final_t3)

def write_op_l():
    """
    write opcode list(10 bit version), generate opcode implement function
    :return: function name list, opcode(long) list
    """
    func_list, op_list_l, func_name_list, diff_type_list = [], [], [], []
    j = 1
    for i, row in csv_data.iterrows():
        if pd.isna(row['ty_am']):
            # blank line
            continue
        if row['name'] == '-':
            # unused opcode
            for _ in range(valtype_num):
                func_list.append('UNUSED()\n')
                op_list_l.append('U_OPCODE(UNUSED{}_L)\n'.format(j))
                j += 1
            continue
        ty_am = row['ty_am'].split('_')

        # function name: Add_s_i32_r_r_r
        opcode_name = row['name'].capitalize()  # "Add"
        ty_am_name = ('_'.join(ty_am[1:])).lower()  # "r_r_r"
        exec_func = 'EXEC_FUNC({})\n'

        has_diff_type = row['diff_type'] == 1;

        spec_valty = False
        for valty in valtype_list:  # normal value type
            if row[valty] == 1:
                func_name = opcode_name + '_' + valty + '_' + ty_am_name
                func_name_filled = func_name
                func_list.append(exec_func.format(func_name_filled))
                final_opcode_name = func_name_filled.upper() + '_L'
                op_list_l.append('U_OPCODE(' + final_opcode_name + ')\n')
                func_name_list.append(func_name_filled)
                spec_valty = True
                if has_diff_type:
                    diff_type_list.append(gen_diff_type_entry(final_opcode_name, row, valty.upper()))
            else:
                # no such valtype for opcode
                func_list.append('UNUSED()\n')
                op_list_l.append('U_OPCODE(UNUSED{}_L)\n'.format(j))
                j += 1
        if not spec_valty:
            func_name = opcode_name + '_' + ty_am_name
            func_list[len(func_list) - valtype_num] = exec_func.format(func_name)
            op_list_l[len(op_list_l) - valtype_num] = 'U_OPCODE(' + func_name.upper() + '_L)\n'
            func_name_list.append(func_name)

            dup_func = 'DUP_FUNC({})\n'.format(func_name)  # duplicate function name
            for k in range(1, valtype_num):
                func_list[len(func_list) - k] = dup_func
                op_list_l[len(op_list_l) - valtype_num + k] = 'U_OPCODE(' + func_name.upper() + str(k) + '_L)\n'
            j -= valtype_num

        # write diff type content
        has_diff_type = row['diff_type']


    write_file(func_list_f, func_list)
    write_file(opcode_list_l_f, op_list_l)
    write_file(diff_type_f, diff_type_list)
    return func_name_list, op_list_l


def write_func_impl(func_name_list):
    """
    write functions to exec_unit.cxx, keep implemented functions, generate if not implemented
    :param func_name_list: functions to implement
    :return:
    """
    impl_funcs = {}  # implemented functions
    lines = None
    with open(impl_f, 'r') as f:
        lines = f.readlines()
    impl_start, impl_end, gen_start, gen_end = 0, 0, 0, 0
    for i in range(0, len(lines)):
        if lines[i].__contains__('// >>>>>>>> implemented code begin >>>>>>>>>>'):
            impl_start = i
        elif lines[i].__contains__('// <<<<<<<<<< implemented code end <<<<<<<<<<'):
            impl_end = i
        elif lines[i].__contains__('// >>>>>>>> generated code begin >>>>>>>>>>\n'):
            gen_start = i
        elif lines[i].__contains__('// <<<<<<<<<< generated code end <<<<<<<<<<\n'):
            gen_end = i
    # collect all instructions implemented
    for i in range(impl_start + 1, impl_end):
        line = lines[i]
        if line.strip().startswith('INLINE void '):
            # INLINE void UWASM_MACHINE::XXX (AM_EXPAND *inst)
            start_idx = line.find("UWASM_MACHINE::") + len("UWASM_MACHINE::")
            end_idx = line.find('(')
            func_name = line[start_idx: end_idx].strip()
            impl_funcs[func_name] = ''  # XXX has been implemented
    # generate un-implemented function
    gen_data = []  # function implement strings
    impl_f_sorted = []  # sorted implemented functions
    impl_f_count = 0  # count implemented function
    for func_name in func_name_list:
        # implemented, no need to generate tmp function
        if func_name in impl_funcs:
            impl_f_sorted.append(impl_funcs[func_name] + '\n')  # sort implemented function
            impl_f_count += 1
            continue
        gen_data.append('INLINE void UWASM_MACHINE::' + func_name +
                        '(AM_EXPAND *inst) { Is_True(false, (UIMPL)); }\n')

    # check if all implemented functions are collected
    if impl_f_count != len(impl_funcs):
        print('{} functions implemented but only {} are in function list'.format(len(impl_funcs), impl_f_count))
    else:
        print('{}/{} functions implemented'.format(impl_f_count, len(func_name_list)))

    # replace generated func bodies and generated cases
    lines = lines[:gen_start+1] + gen_data + lines[gen_end:]

    if lines is not None:
        write_file(impl_f, lines)


def write_cases(opcode_list):
    """
    generate cases
    :param opcode_list: opcode(10 bit version)
    :return:
    """
    lines = None
    with open(case_f, 'r') as f:
        lines = f.readlines()
    impl_start, impl_end = 0, 0
    for i in range(0, len(lines)):
        if lines[i].__contains__('// >>>>>>>> manual case begin >>>>>>>>>>'):
            impl_start = i
        elif lines[i].__contains__('// <<<<<<<<<< manual case end <<<<<<<<<<'):
            impl_end = i
            break
    manual_case = []
    for i in range(impl_start+1, impl_end):
        manual_case.append(lines[i][5:-4])  # collect manual opcode

    case_list, i = [], 0
    while i < len(opcode_list):
        op = opcode_list[i][9:-4]
        if op[:6] == 'UNUSED' or op in manual_case:
            i += 1  # unused, ignore
            continue
        case_str = 'case {}_L:\n'.format(op)
        i += 1
        while i < len(opcode_list) and opcode_list[i][9:-5] == op:  # cases using same function(same opcode, no valtype)
            case_str += 'case {}_L:\n'.format(opcode_list[i][9:-4])
            i += 1
        case_str += '{}(inst);\nbreak;\n'.format(op.capitalize())  # function
        case_list.append(case_str)

    case_list.append('default:\nIs_True(false, ("opcode_l uimpl: 0x%x", inst->Get_opcode_l()));')
    write_file(case_f, lines[:impl_end+1] + case_list)


if __name__ == '__main__':
    print('writing to ' + opcode_list_f + '...')
    write_op_list()
    print('writing to ' + opcode_list_l_f + '...')
    print('writing to ' + func_list_f + '...')
    funcs, opcode = write_op_l()
    print('writing to ' + impl_f + '...')
    write_func_impl(funcs)
    print('writing to ' + case_f + '...')
    write_cases(opcode)
    print('done')
