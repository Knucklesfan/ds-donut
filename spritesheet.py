from PIL import Image
from os import listdir
from os.path import isfile, join

path="/home/knucklesfan/Downloads/spritesheetnew/"
onlyfiles = [f for f in listdir(path) if isfile(join(path, f))]
dst = Image.new('RGBA', (8, len(onlyfiles)*8))
height = 0
for i in onlyfiles:
    im1 = Image.open(path+i)
    dst.paste(im1, (0, height))
    height += 8
dst.save('sheet.png')