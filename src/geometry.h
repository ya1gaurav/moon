/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * geometry.h: Geometry classes
 *
 * Contact:
 *   Moonlight List (moonlight-list@lists.ximian.com)
 *
 * Copyright 2007-2008 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 * 
 */

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <glib.h>

#include <stdint.h>
#include <cairo.h>
#include "enums.h"
#include "rect.h"
#include "transform.h"
#include "moon-path.h"

//
// Geometry
//
/* @Namespace=System.Windows.Media */
class Geometry : public DependencyObject {
 protected:
#if false
	enum GeometryFlags {
		GEOMETRY_NORMAL		= 0x01,	// normal drawing
		GEOMETRY_DEGENERATE	= 0x02,	// degenerate drawing, use the Stroke brush for filling
		GEOMETRY_NEEDS_FILL	= 0x04,	// filling, if specified, is needed (e.g. LineGeometry doesn't need it)
		GEOMETRY_NEEDS_CAPS	= 0x08,	// Stroke[Start|End]LineCap
		GEOMETRY_NEEDS_JOIN	= 0x10,	// StrokeLineJoin, StrokeMiterLimit
		GEOMETRY_MASK		= GEOMETRY_NORMAL | GEOMETRY_DEGENERATE | GEOMETRY_NEEDS_FILL | GEOMETRY_NEEDS_CAPS | GEOMETRY_NEEDS_JOIN
	};
	
	bool IsDegenerate () { return (flags & Geometry::GEOMETRY_DEGENERATE); }
	void SetGeometryFlags (GeometryFlags sf) { flags &= ~Geometry::GEOMETRY_MASK; flags |= sf; }

	int flags;
#endif
	moon_path *path;
	
	virtual ~Geometry ();
	
 public:
 	/* @PropertyType=FillRule,DefaultValue=FillRuleEvenOdd */
	static DependencyProperty *FillRuleProperty;
 	/* @PropertyType=Transform */
	static DependencyProperty *TransformProperty;

	/* @GenerateCBinding,GeneratePInvoke,ManagedAccess=Protected */
	Geometry () : path (NULL) { }
	
	virtual Type::Kind GetObjectType () { return Type::GEOMETRY; }

	virtual void OnPropertyChanged (PropertyChangedEventArgs *args);

	virtual void Draw (Path *path, cairo_t *cr);
	virtual Rect ComputeBounds (Path *path, bool logical) { return Rect (0.0, 0.0, 0.0, 0.0); }
	virtual Rect ComputeBounds (Path *path, bool logical, cairo_matrix_t *matrix) { return ComputeBounds (path, logical); }

	//virtual Point GetOriginPoint (Path *path);

	virtual bool IsFilled () { return true; }

	virtual void Build (Path *path) {}
	virtual bool IsBuilt () { return path && path->cairo.num_data != 0; }
	virtual cairo_path_t *GetCairoPath () { return (path) ? &path->cairo : NULL; }
	
	//
	// Property Accessors
	//
	void SetFillRule (FillRule rule);
	FillRule GetFillRule ();
	
	void SetTransform (Transform *transform);
	Transform *GetTransform ();
};


//
// GeometryCollection
//
/* @Namespace=System.Windows.Media */
class GeometryCollection : public DependencyObjectCollection {
 protected:
	virtual ~GeometryCollection () {}

 public:
	/* @GenerateCBinding,GeneratePInvoke */
	GeometryCollection () { }
	
	virtual Type::Kind GetObjectType () { return Type::GEOMETRY_COLLECTION; }
	virtual Type::Kind GetElementType () { return Type::GEOMETRY; }
};


//
// GeometryGroup
//
/* @ContentProperty="Children" */
/* @Namespace=System.Windows.Media */
class GeometryGroup : public Geometry {
 protected:
	virtual ~GeometryGroup () {}

 public:
 	/* @PropertyType=GeometryCollection */
	static DependencyProperty *ChildrenProperty;
	
	/* @GenerateCBinding,GeneratePInvoke */
	GeometryGroup ();
	
	virtual Type::Kind GetObjectType () { return Type::GEOMETRYGROUP; }
	
	virtual void OnSubPropertyChanged (DependencyProperty *prop, DependencyObject *obj, PropertyChangedEventArgs *subprop_args);
	virtual void OnCollectionItemChanged (Collection *col, DependencyObject *obj, PropertyChangedEventArgs *args);
	virtual void OnCollectionChanged (Collection *col, CollectionChangedEventArgs *args);
	
	virtual void Draw (Path *path, cairo_t *cr);
	virtual Rect ComputeBounds (Path *path, bool logical) { return ComputeBounds (path, logical, NULL); }
	virtual Rect ComputeBounds (Path *path, bool logical, cairo_matrix_t *matrix);
	
	//
	// Property Accessors
	//
	void SetChildren (GeometryCollection *children);
	GeometryCollection *GetChildren ();
};


//
// EllipseGeometry
//
/* @Namespace=System.Windows.Media */
class EllipseGeometry : public Geometry {
 protected:
	virtual void Build (Path *path);
	
	virtual ~EllipseGeometry () {}
	
 public:
 	/* @PropertyType=Point */
	static DependencyProperty *CenterProperty;
 	/* @PropertyType=double,DefaultValue=0.0 */
	static DependencyProperty *RadiusXProperty;
 	/* @PropertyType=double,DefaultValue=0.0 */
	static DependencyProperty *RadiusYProperty;

	/* @GenerateCBinding,GeneratePInvoke */
	EllipseGeometry () { }
	
	virtual Type::Kind GetObjectType () { return Type::ELLIPSEGEOMETRY; }

	virtual Rect ComputeBounds (Path *path, bool logical);
	
	//
	// Property Accessors
	//
	void SetCenter (Point *center);
	Point *GetCenter ();
	
	void SetRadiusX (double radius);
	double GetRadiusX ();
	
	void SetRadiusY (double radius);
	double GetRadiusY ();
};


//
// LineGeometry
//
/* @Namespace=System.Windows.Media */
class LineGeometry : public Geometry {
 protected:
	virtual void Build (Path *path);
	
	virtual ~LineGeometry () {}
	
 public:
 	/* @PropertyType=Point */
	static DependencyProperty *EndPointProperty;
 	/* @PropertyType=Point */
	static DependencyProperty *StartPointProperty;
	
	/* @GenerateCBinding,GeneratePInvoke */
	LineGeometry () { }
	
	virtual Type::Kind GetObjectType () { return Type::LINEGEOMETRY; }
	
	virtual Rect ComputeBounds (Path *path, bool logical);
	
	//
	// Property Accessors
	//
	void SetEndPoint (Point *point);
	Point *GetEndPoint ();
	
	void SetStartPoint (Point *point);
	Point *GetStartPoint ();
};


//
// PathFigureCollection
//
/* @Namespace=System.Windows.Media */
class PathFigureCollection : public DependencyObjectCollection {
 protected:
	virtual ~PathFigureCollection () {}

 public:
	/* @GenerateCBinding,GeneratePInvoke */
	PathFigureCollection () { }
	
	virtual Type::Kind GetObjectType () { return Type::PATHFIGURE_COLLECTION; }
	virtual Type::Kind GetElementType () { return Type::PATHFIGURE; }
};


//
// PathGeometry
//
/* @ContentProperty="Figures" */
/* @Namespace=System.Windows.Media */
class PathGeometry : public Geometry {
	int logical_bounds_available:1;
	int physical_bounds_available:1;
	Rect logical_bounds;
	Rect physical_bounds;
	Rect CacheBounds (Path *path, bool logical, cairo_matrix_t *matrix);
 protected:
	virtual void Build (Path *path);
	
	virtual ~PathGeometry () {}
	
 public:
 	/* @PropertyType=PathFigureCollection */
	static DependencyProperty *FiguresProperty;
	
	/* @GenerateCBinding,GeneratePInvoke */
	PathGeometry ();
	PathGeometry (moon_path *pml_path);

	virtual Type::Kind GetObjectType () { return Type::PATHGEOMETRY; }
	
	virtual void OnCollectionItemChanged (Collection *col, DependencyObject *obj, PropertyChangedEventArgs *args);
	virtual void OnCollectionChanged (Collection *col, CollectionChangedEventArgs *args);
	virtual Rect ComputeBounds (Path *path, bool logical) { return ComputeBounds (path, logical, NULL); }
	virtual Rect ComputeBounds (Path *path, bool logical, cairo_matrix_t *matrix);
	
	// this is an element-by-element decision
	virtual bool IsFilled () { return true; }
	
	//
	// Property Accessors
	//
	void SetFigures (PathFigureCollection *figures);
	PathFigureCollection *GetFigures ();
};


//
// RectangleGeometry
//
/* @Namespace=System.Windows.Media */
class RectangleGeometry : public Geometry {
 protected:
	virtual void Build (Path *path);
	
	virtual ~RectangleGeometry () {}
	
 public:
 	/* @PropertyType=double,DefaultValue=0.0 */
	static DependencyProperty *RadiusXProperty;
 	/* @PropertyType=double,DefaultValue=0.0 */
	static DependencyProperty *RadiusYProperty;
 	/* @PropertyType=Rect */
	static DependencyProperty *RectProperty;
	
	/* @GenerateCBinding,GeneratePInvoke */
	RectangleGeometry () { }
	
	virtual Type::Kind GetObjectType () { return Type::RECTANGLEGEOMETRY; }
	
	virtual Rect ComputeBounds (Path *path, bool logical);
	
	bool GetRadius (double *rx, double *ry);
	
	//
	// Property Accesors
	//
	void SetRadiusX (double radius);
	double GetRadiusX ();
	
	void SetRadiusY (double radius);
	double GetRadiusY ();
	
	void SetRect (Rect *rect);
	Rect *GetRect ();
};


//
// PathSegmentCollection
//
/* @Namespace=System.Windows.Media */
class PathSegmentCollection : public DependencyObjectCollection {
 protected:
	virtual ~PathSegmentCollection () {}

 public:
	/* @GenerateCBinding,GeneratePInvoke */
	PathSegmentCollection () {}
	
	virtual Type::Kind GetObjectType () { return Type::PATHSEGMENT_COLLECTION; }
	virtual Type::Kind GetElementType () { return Type::PATHSEGMENT; }
};


//
// PathFigure
//
/* @ContentProperty="Segments" */
/* @Namespace=System.Windows.Media */
class PathFigure : public DependencyObject {
 protected:
	virtual ~PathFigure ();
	
 public:
 	/* @PropertyType=bool,DefaultValue=false */
	static DependencyProperty *IsClosedProperty;
 	/* @PropertyType=PathSegmentCollection */
	static DependencyProperty *SegmentsProperty;
 	/* @PropertyType=Point */
	static DependencyProperty *StartPointProperty;
	/* @PropertyType=bool,Version=2 */
	static DependencyProperty *IsFilledProperty;
	
	moon_path *path;
	
	/* @GenerateCBinding,GeneratePInvoke */
	PathFigure ();
	
	virtual Type::Kind GetObjectType () { return Type::PATHFIGURE; }
	
	virtual void OnPropertyChanged (PropertyChangedEventArgs *args);
	virtual void OnCollectionItemChanged (Collection *col, DependencyObject *obj, PropertyChangedEventArgs *args);
	virtual void OnCollectionChanged (Collection *col, CollectionChangedEventArgs *args);
	virtual void Build (Path *shape);
	
	virtual bool IsBuilt () { return path && path->cairo.num_data != 0; }
	virtual cairo_path_t *GetCairoPath () { return (path) ? &path->cairo : NULL; }
	
	//
	// Property Accessors
	//
	void SetIsClosed (bool closed);
	bool GetIsClosed ();
	
	void SetSegments (PathSegmentCollection *segments);
	PathSegmentCollection *GetSegments ();
	
	void SetStartPoint (Point *point);
	Point *GetStartPoint ();
};


//
// PathSegment
//
/* @Namespace=System.Windows.Media */
class PathSegment : public DependencyObject {
 protected:
	virtual void Build (Path *path) {}
	
	virtual ~PathSegment () {}
	
 public:
	/* @GenerateCBinding,GeneratePInvoke,ManagedAccess=Protected */
	PathSegment () {}
	
	virtual Type::Kind GetObjectType () { return Type::PATHSEGMENT; }
	
	virtual void Append (moon_path *path) {}
	virtual int GetPathSize () { return 0; }
};


//
// ArcSegment
//
/* @Namespace=System.Windows.Media */
class ArcSegment : public PathSegment {
 protected:
	virtual ~ArcSegment () {}

 public:
 	/* @PropertyType=bool,DefaultValue=false */
	static DependencyProperty *IsLargeArcProperty;
 	/* @PropertyType=Point */
	static DependencyProperty *PointProperty;
 	/* @PropertyType=double,DefaultValue=0.0*/
	static DependencyProperty *RotationAngleProperty;
 	/* @PropertyType=Point,ManagedPropertyType=Size */
	static DependencyProperty *SizeProperty;
 	/* @PropertyType=SweepDirection,DefaultValue=SweepDirectionCounterclockwise */
	static DependencyProperty *SweepDirectionProperty;
	
	/* @GenerateCBinding,GeneratePInvoke */
	ArcSegment () { }
	
	virtual Type::Kind GetObjectType () { return Type::ARCSEGMENT; }
	virtual int GetPathSize () { return 4 * MOON_PATH_CURVE_TO_LENGTH; } // non-optimal size, depends on angle
	
	virtual void Append (moon_path *path);
	
	//
	// Property Accessors
	//
	void SetIsLargeArc (bool large);
	bool GetIsLargeArc ();
	
	void SetPoint (Point *point);
	Point *GetPoint ();
	
	void SetRotationAngle (double angle);
	double GetRotationAngle ();
	
	void SetSize (Point *size);
	Point *GetSize ();
	
	void SetSweepDirection (SweepDirection direction);
	SweepDirection GetSweepDirection ();
};


//
// BezierSegment
//
/* @Namespace=System.Windows.Media */
class BezierSegment : public PathSegment {
 protected:
	virtual ~BezierSegment () {}

 public:
 	/* @PropertyType=Point */
	static DependencyProperty *Point1Property;
 	/* @PropertyType=Point */
	static DependencyProperty *Point2Property;
 	/* @PropertyType=Point */
	static DependencyProperty *Point3Property;
	
	/* @GenerateCBinding,GeneratePInvoke */
	BezierSegment () { }
	
	virtual Type::Kind GetObjectType () { return Type::BEZIERSEGMENT; }
	virtual int GetPathSize () { return MOON_PATH_CURVE_TO_LENGTH; }
	
	virtual void Append (moon_path *path);
	
	//
	// Property Accessors
	//
	void SetPoint1 (Point *point);
	Point *GetPoint1 ();
	
	void SetPoint2 (Point *point);
	Point *GetPoint2 ();
	
	void SetPoint3 (Point *point);
	Point *GetPoint3 ();
};


//
// LineSegment
//
/* @Namespace=System.Windows.Media */
class LineSegment : public PathSegment {
 protected:
	virtual ~LineSegment () {}

 public:
 	/* @PropertyType=Point */
	static DependencyProperty *PointProperty;
	
	/* @GenerateCBinding,GeneratePInvoke */
	LineSegment () { }
	
	virtual Type::Kind GetObjectType () { return Type::LINESEGMENT; }
	virtual int GetPathSize () { return MOON_PATH_LINE_TO_LENGTH; }
	
	virtual void Append (moon_path *path);
	
	//
	// Property Accessors
	//
	void SetPoint (Point *point);
	Point *GetPoint ();
};


//
// PolyBezierSegment
//
/* @Namespace=System.Windows.Media */
class PolyBezierSegment : public PathSegment {
 protected:
	virtual ~PolyBezierSegment () {}
	
	PointCollection *GetPoints ();
	
 public:
 	/* @PropertyType=PointCollection */
	static DependencyProperty *PointsProperty;
	
	/* @GenerateCBinding,GeneratePInvoke */
	PolyBezierSegment () { }
	
	virtual Type::Kind GetObjectType () { return Type::POLYBEZIERSEGMENT; }
	virtual int GetPathSize ();
	
	virtual void Append (moon_path *path);
	
	//
	// Property Accessors
	//
	void SetPoints (PointCollection *points);
};


//
// PolyLineSegment
//
/* @Namespace=System.Windows.Media */
class PolyLineSegment : public PathSegment {
 protected:
	virtual ~PolyLineSegment () {}
	
	PointCollection *GetPoints ();
	
 public:
 	/* @PropertyType=PointCollection */
	static DependencyProperty *PointsProperty;
	
	/* @GenerateCBinding,GeneratePInvoke */
	PolyLineSegment () { }
	
	virtual Type::Kind GetObjectType () { return Type::POLYLINESEGMENT; }
	virtual int GetPathSize ();
	
	virtual void Append (moon_path *path);
	
	//
	// Property Accessors
	//
	void SetPoints (PointCollection *points);
};


//
// PolyQuadraticBezierSegment
//
/* @Namespace=System.Windows.Media */
class PolyQuadraticBezierSegment : public PathSegment {
 protected:
	virtual ~PolyQuadraticBezierSegment () {}
	
	PointCollection *GetPoints ();
	
 public:
 	/* @PropertyType=PointCollection */
	static DependencyProperty *PointsProperty;
	
	/* @GenerateCBinding,GeneratePInvoke */
	PolyQuadraticBezierSegment () { }
	
	virtual Type::Kind GetObjectType () { return Type::POLYQUADRATICBEZIERSEGMENT; }
	virtual int GetPathSize ();

	virtual void Append (moon_path *path);
	
	//
	// Property Accessors
	//
	void SetPoints (PointCollection *points);
};


//
// QuadraticBezierSegment
//
/* @Namespace=System.Windows.Media */
class QuadraticBezierSegment : public PathSegment {
 protected:
	virtual ~QuadraticBezierSegment () {}

 public:
 	/* @PropertyType=Point */
	static DependencyProperty *Point1Property;
 	/* @PropertyType=Point */
	static DependencyProperty *Point2Property;
	
	/* @GenerateCBinding,GeneratePInvoke */
	QuadraticBezierSegment () { }
	
	virtual Type::Kind GetObjectType () { return Type::QUADRATICBEZIERSEGMENT; }
	virtual int GetPathSize () { return MOON_PATH_CURVE_TO_LENGTH; }
	
	virtual void Append (moon_path *path);
	
	//
	// Property Accessors
	//
	void SetPoint1 (Point *point);
	Point *GetPoint1 ();
	
	void SetPoint2 (Point *point);
	Point *GetPoint2 ();
};

#endif
