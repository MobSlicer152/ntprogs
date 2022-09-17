import os
import pathlib
import shutil
import sys
import random
import uuid

def mkuuid():
    uuidbytes = random.randbytes(16)
    return uuid.UUID(bytes_le=uuidbytes)

if len(sys.argv) < 2:
    print(f"{sys.argv[0]} usage: <new name> [uuid]\n" +
          f"UUIDs must be valid as defined here: https://docs.python.org/3/library/uuid.html#uuid.UUID")

name = sys.argv[1]
if len(sys.argv) > 2 and len(sys.argv[2]) == 32:
    projuuid = uuid.UUID(sys.argv[2])
else:
    projuuid = mkuuid()
print(f"Creating project {name} with UUID {{{projuuid}}}")

here = pathlib.Path(os.path.dirname(os.path.realpath(__file__)))
src = here / "template"
dst = here / name

print(f"Copying files from {src} to {dst}")
shutil.copytree(src, dst)
for ext in [ ".c", ".vcxproj", ".vcxproj.filters", ".vcxproj.user" ]:
	print(f"Renaming template{ext} to {name}{ext}")
	shutil.move(dst / f"template{ext}", dst / f"{name}{ext}")

print(f"Filling in blank fields in {name}.vcxproj")
with open(dst / f"{name}.vcxproj", "rb") as file:
	proj = file.read().decode(encoding="utf-8")

proj = proj.replace("<!-- FIXME UUID -->", f"{{{projuuid}}}")
proj = proj.replace("<!-- FIXME name -->", name)
proj = proj.replace("template.c", f"{name}.c")

with open(dst / f"{name}.vcxproj", "wb") as file:
	file.write(bytes(proj, encoding="utf-8"))

print(f"Done. Project \"{name}\" successfully created in {dst}.")
