#set page(paper: "a4", flipped: true, margin: 0.6cm)
#set text(font: "New Computer Modern Sans", size: 10pt)
// #set heading(numbering: "1.")
#show heading: it => [
  #set text(size: 11pt, weight: "bold", fill: blue.darken(20%))
  #v(0.2em)
  #it
  #v(0.2em)
]
#show columns: set box(height: auto)
#show link: it => underline(text(fill: blue.darken(20%))[#it])

#columns(3, gutter: 0.5cm)[
= CS4221 Midterm Cheatsheet 
#emph[by Jonathan Loh, River Koh] (#link("https://github.com/jloh02/nus-notes/tree/main/CS4221")[GitHub])

= L02: Conceptual Modelling (ER)
*ER to Relational Mapping Rules*
- *Strong Entity:* Becomes a table. Attributes become columns. PK remains.
- *Weak Entity:* Table includes its own attributes + PK of the identifying dominant strong entity. PK is the combination of both.
- *1:N Relationship:* Merge relationship into the entity on the 'N' side (add PK of '1' side as Foreign Key).
- *M:N Relationship:* Create a new table. PK is the combination of PKs from participating entities.
- *1:1 Relationship:* Merge into the entity with mandatory participation to minimize NULLs.
- *Nuance - Minimization of NULLs:* Nulls should only exist if they represent missing data meaning, not as an artifact of bad schema translation. Default constraint in SQL should be `NOT NULL` unless explicitly justified.
- *Derived Attributes:* Dotted line

#grid(
      columns: 2,
      gutter: 0mm,
      stack(dir: ttb, spacing: 0pt,
        image("erd-partial-key.png"),
        [Weak entities can only be defined for (1, 1) constraint ]
      ),
      stack(dir: ttb, spacing: 0pt,
        image("erd-composite.png", width:90%),
        image("erd-named.png", width:90%)
      ),
      image("erd-ternary.png"),
      image("erd-aggregate.png"),
)

= L03: Data Warehousing & OLAP
*OLTP vs OLAP*
- *OLTP:* Operational, normalized, short updates, current data. Designed to avoid read-write locks blocking each other. (Typical operational database)
- *OLAP:* Analytical, denormalized, bulk reads/aggregations, historical data (Data warehouses)

*Dimensional Modelling*
- *Fact Tables:* Contains business metrics (measures) and foreign keys to dimension tables. Highly massive.
- *Dimension Tables:* Descriptive attributes. Uses surrogate keys (synthetic) rather than natural keys.
- *Star Schema:* Dimension tables are fully denormalized. Fast queries, simpler joins.
- *Snowflake Schema:* Dimension tables are normalized. Saves space, complex joins.

*OLAP Operations & SQL Nuances*
- *Slice/Dice:* Filtering subsets of a data cube using `WHERE`.
- *SQL `CUBE(A, B)`:* Generates $2^n$ grouping sets: `(A, B)`, `(A)`, `(B)`, `()`.
  - `SELECT product, region, SUM(qty) FROM sales GROUP BY CUBE (product, region)`
- *SQL `ROLLUP(A, B)`:* Generates hierarchical sets: `(A, B)`, `(A)`, `()`. 
  - e.g. `SELECT product, region, SUM(qty) FROM sales GROUP BY ROLLUP (product, region)`
- In `ROLLUP`/`CUBE` output, aggregated columns for subtotals will appear as *`NULL`*.
- *Drill-Down:* Implemented using `GROUP BY`

#image("rollup-drilldown.png")

= L04: Relational Algebra
*Basic Operations*
- *Selection ($sigma_c$):* Filters rows based on condition $c$.
- *Projection ($pi_L$):* Extracts specific columns $L$. Removes duplicates automatically in relational algebra (pure sets).
- *Cross Product ($times$):* Cartesian product. $R times S$ has $n+m$ columns and $n times m$ rows.
- *Rename Attr:* $rho_(b arrow.l a)$
- *Rename Relation:* $rho(R,S)$

*Set Operations (Requires union-compatibility)*
- *`UNION` ($union$):* Combines tuples, removing duplicates.
- *`INTERSECT` ($inter$):* Tuples present in both $R$ and $S$.
- *Minus / Difference `EXCEPT` ($backslash$):* $R backslash S$ returns tuples in $R$ but not in $S$.

*Join Operations*
- *Theta Join ($join_theta$):* $R join_theta S equiv sigma_(theta)(R times S)$.
- *Natural Join ($join$):* Equi-join on all matching column names, keeping only one copy of matching columns.
- *Left Outer Join ($join.l$):* Keeps all rows from the left table. Unmatched right columns become `NULL`.
- *Right Outer Join ($join.r$):* Keeps all rows from right table.
- *Full Outer Join (\u{27d7}):* Keeps all rows from both tables, filling `NULL`s where there are no matches.
- *Semi-Join ($times.l$):* Returns rows in $R$ that have a match in $S$. (Optimized for `EXISTS`).
- *Anti-Join ($triangle.r$):* Returns rows in $R$ that DO NOT have a match in $S$. (Optimized for `NOT EXISTS`).

#align(center)[
  #image("joins-2.png", width:90%)
]

*Equivalence & Optimization*
- *Selection Pushdown:* $sigma_c (R join S) equiv sigma_c (R) join S$ (if condition $c$ only applies to $R$). Reduces rows early!
- *Projection Distribution:* $pi_L (E_1 union E_2) equiv pi_L (E_1) union pi_L (E_2)$.

= L05: SQL Tuning I (Indexes)
*EXPLAIN & Query Plans*
- `EXPLAIN`: Est. startup..total cost, rows, width in Bytes
#image("exec-plan-output.png")
- `EXPLAIN (ANALYZE)`: Actual execution times, rows, loops, filtered rows for each filter
#image("exec-explain-analyze.png")
- *Temporary Table* : `CREATE TEMPORARY TABLE stocks_agg AS SELECT * FROM stocks WHERE ...`
  - No indexes, no implicit statistics (need `ANALYZE temp_t`)

// #image("exec-plan.png")


*Catalogue and Statistics*
- *`pg_tables`*: name, index, triggers in DB
- *`pg_attribute`*: table columns, one row for every column
- *`pg_statistics`*: created by `ANALYZE`, approximate statistics about contents in DB
- *`pg_stats`*: allows access to rows of `pg_statistics` that user has permissions for to read

*Scan Types*
- *Sequential Scan:* Reads every page.
- *Index Scan:* Traverses B-Tree, then fetches rows from the heap. Causes random I/O.
- *Index-Only Scan:* Finds *all* requested columns directly in the index. No heap fetch!
- *Covering Index:* `CREATE INDEX idx ON T(A) INCLUDE (B)`
- *Predicate Index:* `CREATE INDEX idx ON T(A) WHERE A > 600`

= L06: SQL Tuning II & Maintenance


*Join Implementations*
- *Nested Loop Join:* Outer table scanned once, inner table scanned for every outer row. Good if inner table is indexed.
- *Hash Join:* Builds a hash table on the inner relation. Favored for large unsorted tables. Requires `work_mem`.
- *Merge Join:* Sorts both inputs, then merges. Best if inputs are already sorted by B-Tree indexes.

*Table Maintenance*
- *VACUUM:* Reclaims space from dead tuples (MVCC artifacts).
- *VACUUM FULL:* Rewrites table to disk, shrinking file. Takes an exclusive lock.
- *CLUSTER:* Rewrites table so physical order matches an index.

= Common SQL Nuances & Midterm Specifics
*`DROP` vs `DELETE` vs `TRUNCATE`*
- *`DELETE` (DML):* Removes rows row-by-row. Logs each deletion. Slower, but triggers fire. Can use `WHERE`.
- *`TRUNCATE` (DDL-like DML):* Deletes all rows instantly by deallocating pages. No triggers fired. Resets `IDENTITY`/`SERIAL` columns. Cannot use `WHERE`.
- *`DROP TABLE` (DDL):* Completely removes the table data, structure, indexes, and privileges.

*`EXCEPT` vs `NOT IN` vs `NOT EXISTS`*
- `EXCEPT`: Set operation, automatically removes duplicates.
- `NOT IN`: *Danger:* Fails completely (returns 0 rows) if the subquery returns ANY `NULL` values.
- `NOT EXISTS`: Safer, usually optimized into an Anti-Join.

*Other Traps*
- *NULL Comparisons:* `x = NULL` evaluates to `UNKNOWN` (treated as false in `WHERE`). Always use `x IS NULL`.
- *`WHERE` vs `HAVING`:* `WHERE` filters rows *before* aggregation. `HAVING` filters groups *after* aggregation.
- *`UNION` vs `UNION ALL`:* `UNION` sorts and removes duplicates (expensive). Use `UNION ALL` if duplicates are acceptable or impossible.
- *Execution Order:* `FROM` $arrow.r$ `WHERE` $arrow.r$ `GROUP BY` $arrow.r$ `HAVING` $arrow.r$ `SELECT` $arrow.r$ `ORDER BY`. (You cannot use an alias defined in `SELECT` within the `WHERE` clause!).


= Cross-only Join Cheatsheet (RA)

*Core Trick*
- *Theta Join:* $R join_theta S equiv sigma_(theta)(R times S)$
- *Equijoin:* $R join_(R.a = S.b) S equiv sigma_(R.a = S.b)(R times S)$

*Natural Join (only using $times$, $sigma$, $pi$, $rho$)*
Let $R(A,B)$ and $S(B,C)$.
- Rename to avoid duplicate attribute names:
  $S' := rho_(B' arrow.l B)(S)$
- Cross + select on equality:
  $T := sigma_(R.B = S'.B')(R times S')$
- Drop duplicate join column:
  $R join S equiv pi_(A, B, C)(T)$

*Self-Join (needs rename)*
- $sigma_(theta)( rho_("R1")(R) times rho_("R2")(R) )$

*Semi-Join (EXISTS / `ANY` vs `IN`) from cross join*
- Definition idea: join then keep only left attrs
- $R times.l_(theta) S equiv delta( pi_((R))( sigma_(theta)(R times S) ) )$

*Anti-Join (`NOT EXISTS` / `LEFT JOIN `) from cross join*
- $R triangle.r_(theta) S equiv R backslash (R times.l_(theta) S)$

*Assignment 1 identities*

- $R ⟕_["cond"] S equiv delta( sigma_["cond"](R times S) ) union (R ▷_["cond"] S)$
- $R ÷ S equiv pi_[R - S](R) backslash ( pi_[R - S]( pi_[R]( pi_[R - S](R) times S ) backslash R ) )$
]
