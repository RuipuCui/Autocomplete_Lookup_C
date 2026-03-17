# Autocomplete Lookup (C)

Implements autocomplete/prefix-search over a Melbourne business dataset using three progressively more efficient data structures written in C: a **linked list**, a **sorted array with binary search**, and a **radix (Patricia) tree**.

---

## Project Structure

```
.
├── data.c / data.h       # Dataset struct & CSV parsing
├── list.c / list.h       # Singly-linked list (Stage 1)
├── array.c / array.h     # Sorted dynamic array (Stage 2)
├── tree.c / tree.h       # Radix (Patricia) tree (Stage 3)
├── dict1.c               # Stage 1 entry point – linked list search
├── dict2.c               # Stage 2 entry point – sorted array + binary search
├── dict3.c               # Stage 3 entry point – radix tree search
├── Makefile
└── tests/
    ├── dataset_1.csv      # 1-record dataset
    ├── dataset_2.csv
    ├── dataset_20.csv
    ├── dataset_100.csv
    ├── dataset_1000.csv   # 1000-record dataset
    ├── test1.in           # Query file for dataset_1
    ├── test2.in
    ├── test20.in
    ├── test100.in
    └── test1000.in
```

---

## Dataset

The CSV files contain Melbourne business (CLUE) data with the following 14 fields:

| Field | Type | Description |
|---|---|---|
| `census_year` | int | Year the record was collected |
| `block_id` | int | City block ID |
| `property_id` | int | Property ID |
| `base_property_id` | int | Building ID |
| `building_address` | string | Address of the building |
| `clue_small_area` | string | CLUE area of Melbourne |
| `business_address` | string | Address of the business |
| `trading_name` | string | **Search key** – name of the business |
| `industry_code` | int | Industry category ID |
| `industry_description` | string | Industry category description |
| `seating_type` | string | Type of seating |
| `number_of_seats` | int | Number of seats |
| `longitude` | double | Longitude of seating location |
| `latitude` | double | Latitude of seating location |

Searches are performed on the `trading_name` field.

---

## Building

```bash
make dict1   # Stage 1 – linked list
make dict2   # Stage 2 – sorted array
make dict3   # Stage 3 – radix tree
```

Or build all at once:

```bash
make dict1 dict2 dict3
```

---

## Usage

Each executable takes three arguments and reads search queries from stdin (one per line):

```bash
./dict<N> <stage> <dataset.csv> <output.txt> < <queries.in>
```

| Argument | Description |
|---|---|
| `<stage>` | Stage number: `1`, `2`, or `3` |
| `<dataset.csv>` | Path to the input CSV file |
| `<output.txt>` | Path to the output file |

### Examples

```bash
# Stage 1 – linear search via linked list
./dict1 1 tests/dataset_20.csv output.txt < tests/test20.in

# Stage 2 – binary search on sorted array
./dict2 2 tests/dataset_100.csv output.txt < tests/test100.in

# Stage 3 – radix tree prefix search
./dict3 3 tests/dataset_1000.csv output.txt < tests/test1000.in
```

---

## Implementation Details

### Stage 1 – Linked List (`dict1`)
- Loads all records into a singly-linked list.
- Performs a **linear scan** to find all records whose `trading_name` exactly matches the query.
- Comparison metrics reported: string comparisons and character comparisons.

### Stage 2 – Sorted Array (`dict2`)
- Inserts records into a **dynamically-resizing sorted array** (initial size 2, doubles on overflow).
- Uses **binary search** to locate the first matching prefix, then a **linear scan** forward to collect all matches.
- Array is shrunk to fit after loading (`arrayShrink`).
- Comparison metrics reported: string comparisons, character comparisons, and array size.

### Stage 3 – Radix (Patricia) Tree (`dict3`)
- Builds a **bit-level radix tree** over `trading_name` keys.
- Traverses the tree bit-by-bit to find all records sharing the query as a prefix.
- Comparison metrics reported: string comparisons, character comparisons, and bit comparisons.

---

## Performance Comparison

The three approaches offer different time complexities for prefix lookup:

| Stage | Structure | Search Complexity |
|---|---|---|
| 1 | Linked list | O(n) |
| 2 | Sorted array | O(log n + k) |
| 3 | Radix tree | O(m) per query, where m = prefix length in bits |

Use the provided test datasets (1 → 1000 records) to benchmark and compare each implementation.
