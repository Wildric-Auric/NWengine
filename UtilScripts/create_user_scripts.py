import sys
import os

def create_files(class_name, directory):
    # Ensure the directory exists
    os.makedirs(directory, exist_ok=True)

    # Define the content for the files
    s0_content = f"""#pragma "once"
#include "NWengineCore.h"

class {class_name}: public Scriptable {{
public:
    SCRIPT_CONSTR({class_name})
    void Start()  override;
    void Update() override;
}};
"""

    s1_content = f"""#include "{class_name}.h"

void {class_name}::Start() {{

}}

void {class_name}::Update() {{

}}
"""

    s0_file = os.path.join(directory, f"{class_name}.h")
    s1_file = os.path.join(directory, f"{class_name}.cpp")

    # Write to s0 file
    with open(s0_file, 'w') as file:
        file.write(s0_content)

    # Write to s1 file
    with open(s1_file, 'w') as file:
        file.write(s1_content)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: create_user_scripts.py <ClassName> <Directory>")
        sys.exit(1)

    # Get the class name and directory from the arguments
    class_name = sys.argv[1]
    directory = sys.argv[2]

    # Call the function to create the files
    create_files(class_name, directory)

