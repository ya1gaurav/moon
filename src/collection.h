/*
 * collection.h: different types of collections
 *
 * Copyright 2007 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 * 
 */

#ifndef __MOON_COLLECTION_H__
#define __MOON_COLLECTION_H__

#include "dependencyobject.h"
#include "list.h"

//
// Collection: provides a collection that we can monitor for
// changes.   We expose this collection in a few classes to
// the managed world, and when a change happens we get a
// chance to reflect the changes
//
class Collection : public DependencyObject {
 public:
	class Node : public List::Node {
	public:
		DependencyObject *obj;
		
		Node (DependencyObject *dob, DependencyObject *parent);
		~Node ();
	};
	
	List *list;
	void *closure;
	
	Collection ();
	virtual ~Collection ();
	virtual Type::Kind GetObjectType () { return Type::COLLECTION; };	
	virtual Type::Kind GetElementType () { return Type::DEPENDENCY_OBJECT; }

	virtual void Add    (DependencyObject *data);
	virtual void Remove (DependencyObject *data);
	virtual void Insert (int index, DependencyObject *data);
	virtual void Clear  ();

 private:
	void SharedAdd (DependencyObject *data);
};

bool CollectionNodeFinder (List::Node *n, void *data);

class CollectionIterator {
 public:
	CollectionIterator (Collection *c){
		first = true;
		collection = c;
		current = c->list->First ();
	}

	bool first;
	Collection *collection;
	List::Node *current;
};

class VisualCollection : public Collection {
 public:
	VisualCollection ();
	virtual ~VisualCollection ();
	virtual Type::Kind GetObjectType () { return Type::VISUAL_COLLECTION; }
	virtual Type::Kind GetElementType () { return Type::VISUAL; }

	virtual void Add    (DependencyObject *data);
	virtual void Remove (DependencyObject *data);
	virtual void Insert (int index, DependencyObject *data);
	virtual void Clear  ();

	void ResortByZIndex ();
	List *z_sorted_list;

 private:
	void VisualUpdate (DependencyObject *data);
};

class TriggerCollection : public Collection {
 public:
	TriggerCollection () {}
	virtual Type::Kind GetObjectType () { return Type::TRIGGER_COLLECTION; }
	virtual Type::Kind GetElementType () { return Type::EVENTTRIGGER; }

	virtual void Add    (DependencyObject *data);
	virtual void Remove (DependencyObject *data);
	virtual void Insert (int index, DependencyObject *data);
};

class TriggerActionCollection : public Collection {
 public:
	TriggerActionCollection () {}
	virtual Type::Kind GetObjectType () { return Type::TRIGGERACTION_COLLECTION; }
	/* this may seem wrong, but it's what the TriggerActionCollection mandates */
	virtual Type::Kind GetElementType () { return Type::BEGINSTORYBOARD; }
};

class ResourceCollection : public Collection {
 public:
	ResourceCollection () {}
	virtual Type::Kind GetObjectType () { return Type::RESOURCE_COLLECTION; }
	virtual Type::Kind GetElementType () { return Type::DEPENDENCY_OBJECT; }

	virtual void Add    (DependencyObject *data);
	virtual void Remove (DependencyObject *data);
};

class StrokeCollection : public Collection {
 public:
	StrokeCollection () {}
	virtual Type::Kind GetObjectType () { return Type::STROKE_COLLECTION; }
	virtual Type::Kind GetElementType () { return Type::STROKE; }

	virtual void Add    (DependencyObject *data);
	virtual void Remove (DependencyObject *data);
};

class StylusPointCollection : public Collection {
 public:
	StylusPointCollection () {}
	virtual Type::Kind GetObjectType () { return Type::STYLUSPOINT_COLLECTION; }
	virtual Type::Kind GetElementType () { return Type::STYLUSPOINT; }

	virtual void Add    (DependencyObject *data);
	virtual void Remove (DependencyObject *data);
};

class TimelineMarkerCollection : public Collection {
 public:
	TimelineMarkerCollection () {}
	virtual Type::Kind GetObjectType () { return Type::TIMELINEMARKER_COLLECTION; }
	virtual Type::Kind GetElementType () { return Type::TIMELINEMARKER; }

	virtual void Add    (DependencyObject *data);
	virtual void Remove (DependencyObject *data);
};

class MediaAttributeCollection : public Collection {
 public:
	MediaAttributeCollection () {}
	virtual Type::Kind GetObjectType () { return Type::MEDIAATTRIBUTE_COLLECTION; }
	virtual Type::Kind GetElementType () { return Type::MEDIAATTRIBUTE; }

	virtual void Add    (DependencyObject *data);
	virtual void Remove (DependencyObject *data);
};

class Inlines : public Collection {
 public:
	Inlines () {}
	virtual Type::Kind GetObjectType () { return Type::INLINES; }
	virtual Type::Kind GetElementType () { return Type::INLINE; }

	virtual void Add    (DependencyObject *data);
	virtual void Remove (DependencyObject *data);
};

G_BEGIN_DECLS

void collection_add    (Collection *collection, DependencyObject *data);
void collection_remove (Collection *collection, DependencyObject *data);
void collection_insert (Collection *collection, int index, DependencyObject *data);
void collection_clear  (Collection *collection);
int  collection_count  (Collection *collection);

DependencyObject   *collection_get_value_at (Collection *collection, int index);
Type::Kind          collection_get_element_type (Collection *collection);
CollectionIterator *collection_get_iterator (Collection *collection);

bool   collection_iterator_move_next   (CollectionIterator *iterator);
void   collection_iterator_reset       (CollectionIterator *iterator);
void   collection_iterator_destroy     (CollectionIterator *iterator);
DependencyObject *collection_iterator_get_current (CollectionIterator *iterator);


Collection *collection_new (Type::Kind kind);

VisualCollection *visual_collection_new (void);
TriggerCollection *trigger_collection_new (void);
TriggerActionCollection *trigger_action_collection_new (void);
ResourceCollection *resource_collection_new (void);
StrokeCollection *stroke_collection_new (void);
StylusPointCollection *stylus_point_collection_new (void);
TimelineMarkerCollection *timeline_marker_collection_new (void);
GradientStopCollection *gradient_stop_collection_new (void);
MediaAttributeCollection *media_attribute_collection_new (void);
Inlines *inlines_new (void);

G_END_DECLS

#endif /* __MOON_COLLECTION_H__ */
