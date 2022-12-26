xcrun -sdk macosx metal -c src/matop.metal -o build/darwin/matop.air
xcrun -sdk macosx metallib build/darwin/matop.air -o build/darwin/matop.metallib