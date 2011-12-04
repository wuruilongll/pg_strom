/*
 * pg_strom.h
 *
 * Header file of pg_strom module
 *
 * --
 * Copyright 2011-2012 (c) KaiGai Kohei <kaigai@kaigai.gr.jp>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the 'LICENSE' included within
 * this package.
 */
#ifndef PG_STROM_H
#define PG_STROM_H

#include "catalog/indexing.h"
#include "commands/explain.h"
#include "fmgr.h"
#include "foreign/fdwapi.h"
#include "pg_strom_cuda.h"

#define PGSTROM_SCHEMA_NAME		"pg_strom"

#define PGSTROM_CHUNK_SIZE		(2400 * (BLCKSZ / 8192))

/*
 * utilcmds.c
 */
extern void
pgstrom_utilcmds_init(void);

/*
 * blkload.c
 */
extern Datum pgstrom_data_load(PG_FUNCTION_ARGS);
extern Datum pgstrom_data_clear(PG_FUNCTION_ARGS);
extern Datum pgstrom_data_compaction(PG_FUNCTION_ARGS);

/*
 * plan.c
 */
extern FdwPlan *
pgstrom_plan_foreign_scan(Oid foreignTblOid,
			  PlannerInfo *root, RelOptInfo *baserel);
extern void
pgstrom_explain_foreign_scan(ForeignScanState *node, ExplainState *es);

/*
 * scan.c
 */
typedef struct {
	Relation	base_rel;
	Relation	usemap_rel;
	Relation   *column_rel;
	CatalogIndexState	usemap_idx;
	CatalogIndexState  *column_idx;
	Oid			sequence_id;
} RelationSetData;
typedef RelationSetData *RelationSet;

extern RelationSet
pgstrom_open_relation_set(Oid base_relid, LOCKMODE lockmode);
extern void
pgstrom_close_relation_set(RelationSet relset, LOCKMODE lockmode);

extern void
pgstrom_begin_foreign_scan(ForeignScanState *fss, int eflags);
extern TupleTableSlot*
pgstrom_iterate_foreign_scan(ForeignScanState *fss);
extern void
pgboost_rescan_foreign_scan(ForeignScanState *fss);
extern void
pgboost_end_foreign_scan(ForeignScanState *fss);

/*
 * pg_strom.c
 */
extern FdwRoutine pgstromFdwHandlerData;
extern Datum pgstrom_fdw_handler(PG_FUNCTION_ARGS);
extern Datum pgstrom_fdw_validator(PG_FUNCTION_ARGS);

#endif	/* PG_STROM_H */