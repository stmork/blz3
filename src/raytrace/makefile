# Beispiel Makefile

.KEEP_STATE:

# Optionen

CPPFLAGS		= $(STDINC) -I$(BLZ3_INCLUDE)
LDLIBS			= $(LIB_B3) $(LIB_X3) $(LIB_S3)
PRGS			= 
LIBS			= $(BLZ3_LIB)/libB3Raytrace.a
PRG_SOURCE		=\
	b3Animation.cc\
	b3Area.cc\
	b3BBox.cc\
	b3Box.cc\
	b3Bump.cc\
	b3Condition.cc\
	b3Cone.cc\
	b3CSGBox.cc\
	b3CSGCone.cc\
	b3CSGCylinder.cc\
	b3CSGEllipsoid.cc\
	b3CSGShape.cc\
	b3CSGSphere.cc\
	b3CSGTorus.cc\
	b3Cylinder.cc\
	b3Disk.cc\
	b3Ellipsoid.cc\
	b3Init.cc\
	b3Intersect.cc\
	b3Light.cc\
	b3Material.cc\
	b3Normal.cc\
	b3NormalDeriv.cc\
	b3Raytrace.cc\
	b3RenderShape.cc\
	b3RenderView.cc\
	b3Scene.cc\
	b3Shade.cc\
	b3ShadeMork.cc\
	b3ShadePhong.cc\
	b3Shape.cc\
	b3Special.cc\
	b3Sphere.cc\
	b3SplineArea.cc\
	b3SplineCylinder.cc\
	b3SplineRing.cc\
	b3SplineRot.cc\
	b3SplineShape.cc\
	b3Torus.cc\
	b3Triangles.cc\
	b3TriangleShape.cc

PRG_OBJ			=\
	b3Animation.o\
	b3Area.o\
	b3BBox.o\
	b3Box.o\
	b3Bump.o\
	b3Condition.o\
	b3Cone.o\
	b3CSGBox.o\
	b3CSGCone.o\
	b3CSGCylinder.o\
	b3CSGEllipsoid.o\
	b3CSGShape.o\
	b3CSGSphere.o\
	b3CSGTorus.o\
	b3Cylinder.o\
	b3Disk.o\
	b3Ellipsoid.o\
	b3Init.o\
	b3Intersect.o\
	b3Light.o\
	b3Material.o\
	b3Normal.o\
	b3NormalDeriv.o\
	b3Raytrace.o\
	b3RenderShape.o\
	b3RenderView.o\
	b3Scene.o\
	b3Shade.o\
	b3ShadeMork.o\
	b3ShadePhong.o\
	b3Shape.o\
	b3Special.o\
	b3Sphere.o\
	b3SplineArea.o\
	b3SplineCylinder.o\
	b3SplineRing.o\
	b3SplineRot.o\
	b3SplineShape.o\
	b3Torus.o\
	b3Triangles.o\
	b3TriangleShape.o

all:	lib

depend:
#	makedepend -- $(CPPFLAGS) -- $(PRG_SOURCE)
	gcc -M $(PRG_SOURCE) $(CPPFLAGS) >.dep

clean:
	rm -f *.o *.a *.bak *~ core $(PRGS) $(LIBS) ALL QUIET

lib:	$(LIBS)

$(LIBS):	$(PRG_OBJ)
	rm -f $@ core
	$(AR)     $(ARFLAGS) $@ $^
	$(RANLIB) $@

-include .dep
