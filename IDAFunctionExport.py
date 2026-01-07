import idautils
import idc
import re

output_file = "funcs.log"
with open(output_file, "w") as f:
    for func_ea in idautils.Functions():
        name = idc.get_func_name(func_ea)
        func_type = idc.get_type(func_ea)
        
        if func_type:
            match = re.match(r'(.+?)\s+(__\w+)\((.*)\)', func_type)
            if match:
                return_type = match.group(1).strip()
                calling_conv = match.group(2).strip()
                params = match.group(3).strip()
                line = f"{return_type} {calling_conv} 0x{func_ea:08X} {name}({params})\n"
            else:
                line = f"{func_type} 0x{func_ea:08X} {name}\n"
        else:
            line = f"void 0x{func_ea:08X} {name}\n"
        
        f.write(line)

print(f"Function list saved to {output_file}")