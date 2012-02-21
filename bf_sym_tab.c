#include "bf_sym_tab.h"

void populate_sym_table(struct binary_file * bf, asymbol * sym)
{
	struct bf_sym * entry = xmalloc(sizeof(struct bf_sym));
	symbol_info	info;

	bfd_symbol_info(sym, &info);

	entry->name = xstrdup(sym->name);
	entry->vma  = info.value;
	htable_add(&bf->sym_table, &entry->entry, &entry->vma,
			sizeof(entry->vma));
}

void load_sym_table(struct binary_file * bf)
{
	binary_file_for_each_symbol(bf, populate_sym_table);
}

struct bf_sym * bf_get_sym(struct binary_file * bf, bfd_vma vma)
{
	return hash_find_entry(&bf->sym_table, &vma, sizeof(vma),
			struct bf_sym, entry);
}

void bf_close_sym_table(struct binary_file * bf)
{
	struct htable_entry * cur_entry;
	struct htable_entry * n;
	struct bf_sym *	      sym;

	htable_for_each_entry_safe(sym, cur_entry, n, &bf->sym_table, entry) {
		htable_del_entry(&bf->sym_table, cur_entry);
		free(sym->name);
		free(sym);
	}
}
