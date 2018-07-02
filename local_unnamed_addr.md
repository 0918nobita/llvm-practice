# ``local_unnamed_addr`` 属性

もし ``local_unnamed_addr`` 属性がグローバルに付与されていたら、そのアドレスはモジュール内では重要ではないと認識される。それは、``unnamed_attr`` 属性とは区別されます。

If a ``local_unnamed_addr`` attribute is attached to a global, the address is known to be insignificant within the module. It is distinct from the exisiting ``unnamed_attr`` attribute in that it only describes a local property of the module rather than a global property of the symbol.

This attribute is intended to be used by the code generator and LTO to allow the linker to decide whether the global needs to be in the symbol table. It is possible to exclude a global from the symbol table if three things are true:

- This attribute is present on every instance of the global (which means that the normal rule that the global must have a unique address can be broken without being observable by the program by performing comparisons against the global's address)

- The global has ``linkonce_odr`` linkage (which means that each linkage unit must have its own copy of the global if it requires one, and the copy in each linkage unit must be the same)

- It is a constant or a function (which means that the program cannot observe that the unique-address rule has been broken by writing to the global)

Although this attribute could in principle be computed from the module contents, LTO clients (i.e. linkers) will normally need to be able to compute this property as part of symbol resolution, and it would be inefficient to materialize every module just to compute it.

