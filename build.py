import zipfile
import os
import secrets

with zipfile.ZipFile('build_' + str(secrets.randbelow(1000000000)) + ".zip", 'w', zipfile.ZIP_DEFLATED) as zipf:
    zipf.write('app.py')
    for dirname, subdirs, files in os.walk("frontend/build"):
        zipf.write(dirname)
        for filename in files:
            zipf.write(os.path.join(dirname, filename))
zipf.close()