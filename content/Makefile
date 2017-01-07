OUTDIR=out

all: out/textured.gl out/export out/mp_grid

out/textured.gl: Shaders/textured.gl.frag Shaders/textured.gl.vert 
	cp Shaders/textured.gl.frag $(OUTDIR)/textured.gl.frag
	cp Shaders/textured.gl.vert $(OUTDIR)/textured.gl.vert

out/export:
	cp Art/export/* $(OUTDIR)/

out/mp_grid: Levels/mp_grid.tmx
	tiled --export-map Levels/mp_grid.tmx $(OUTDIR)/mp_grid.json