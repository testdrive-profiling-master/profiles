docgen.doxygen			= {}
docgen.doxygen.config	= {}

do
	local config	= docgen.doxygen.config
	config.DOXYFILE_ENCODING		= "UTF-8"
	config.PROJECT_NAME				= "Doxygen"
	config.PROJECT_NUMBER			= "1.0"
	config.PROJECT_BRIEF			= "This is the synopsis"
	config.PROJECT_LOGO				= ""
	config.PROJECT_ICON				= ""
	config.OUTPUT_DIRECTORY			= "output"
	config.CREATE_SUBDIRS			= false
	config.CREATE_SUBDIRS_LEVEL		= 8
	config.ALLOW_UNICODE_NAMES		= false
	config.OUTPUT_LANGUAGE			= "Korean-en"
	config.BRIEF_MEMBER_DESC		= true
	config.REPEAT_BRIEF				= true
	config.ABBREVIATE_BRIEF			= {"The $name class",
						 "The $name widget",
						 "The $name file",
						 "is",
						 "provides",
						 "specifies",
						 "contains",
						 "represents",
						 "a",
						 "an",
						 "the"}
	config.ALWAYS_DETAILED_SEC		= true
	config.INLINE_INHERITED_MEMB	= false
	config.FULL_PATH_NAMES			= true
	config.STRIP_FROM_PATH			= ""
	config.STRIP_FROM_INC_PATH		= ""
	config.SHORT_NAMES				= false
	config.JAVADOC_AUTOBRIEF		= false
	config.JAVADOC_BANNER			= false
	config.QT_AUTOBRIEF				= false
	config.MULTILINE_CPP_IS_BRIEF	= false
	config.PYTHON_DOCSTRING			= true
	config.INHERIT_DOCS				= true
	config.SEPARATE_MEMBER_PAGES	= false
	config.TAB_SIZE					= 4
	config.ALIASES					= ""
	config.OPTIMIZE_OUTPUT_FOR_C	= false
	config.OPTIMIZE_OUTPUT_JAVA		= false
	config.OPTIMIZE_FOR_FORTRAN		= false
	config.OPTIMIZE_OUTPUT_VHDL		= false
	config.OPTIMIZE_OUTPUT_SLICE	= false
	config.EXTENSION_MAPPING		= ""
	config.MARKDOWN_SUPPORT			= true
	config.TOC_INCLUDE_HEADINGS		= 5
	config.MARKDOWN_ID_STYLE		= "DOXYGEN"
	config.AUTOLINK_SUPPORT			= true
	config.BUILTIN_STL_SUPPORT		= false
	config.CPP_CLI_SUPPORT			= false
	config.SIP_SUPPORT				= false
	config.IDL_PROPERTY_SUPPORT		= true
	config.DISTRIBUTE_GROUP_DOC		= false
	config.GROUP_NESTED_COMPOUNDS	= false
	config.SUBGROUPING				= true
	config.INLINE_GROUPED_CLASSES	= false
	config.INLINE_SIMPLE_STRUCTS	= false
	config.TYPEDEF_HIDES_STRUCT		= false
	config.LOOKUP_CACHE_SIZE		= 0
	config.NUM_PROC_THREADS			= 1
	config.TIMESTAMP				= false
	config.EXTRACT_ALL				= true
	config.EXTRACT_PRIVATE			= false
	config.EXTRACT_PRIV_VIRTUAL		= false
	config.EXTRACT_PACKAGE			= false
	config.EXTRACT_STATIC			= false
	config.EXTRACT_LOCAL_CLASSES	= true
	config.EXTRACT_LOCAL_METHODS	= false
	config.EXTRACT_ANON_NSPACES		= false
	config.RESOLVE_UNNAMED_PARAMS	= true
	config.HIDE_UNDOC_MEMBERS		= false
	config.HIDE_UNDOC_CLASSES		= false
	config.HIDE_FRIEND_COMPOUNDS	= false
	config.HIDE_IN_BODY_DOCS		= false
	config.INTERNAL_DOCS			= false
	config.CASE_SENSE_NAMES			= false
	config.HIDE_SCOPE_NAMES			= false
	config.HIDE_COMPOUND_REFERENCE	= false
	config.SHOW_HEADERFILE			= true
	config.SHOW_INCLUDE_FILES		= true
	config.SHOW_GROUPED_MEMB_INC	= false
	config.FORCE_LOCAL_INCLUDES		= false
	config.INLINE_INFO				= true
	config.SORT_MEMBER_DOCS			= true
	config.SORT_BRIEF_DOCS			= false
	config.SORT_MEMBERS_CTORS_1ST	= false
	config.SORT_GROUP_NAMES			= false
	config.SORT_BY_SCOPE_NAME		= false
	config.STRICT_PROTO_MATCHING	= false
	config.GENERATE_TODOLIST		= true
	config.GENERATE_TESTLIST		= true
	config.GENERATE_BUGLIST			= true
	config.GENERATE_DEPRECATEDLIST	= true
	config.ENABLED_SECTIONS			= ""
	config.MAX_INITIALIZER_LINES	= 30
	config.SHOW_USED_FILES			= true
	config.SHOW_FILES				= true
	config.SHOW_NAMESPACES			= true
	config.FILE_VERSION_FILTER		= ""
	config.LAYOUT_FILE				= ""
	config.CITE_BIB_FILES			= ""
	config.EXTERNAL_TOOL_PATH		= ""
	config.QUIET					= false
	config.WARNINGS					= true
	config.WARN_IF_UNDOCUMENTED		= true
	config.WARN_IF_DOC_ERROR		= true
	config.WARN_IF_INCOMPLETE_DOC	= true
	config.WARN_NO_PARAMDOC			= false
	config.WARN_IF_UNDOC_ENUM_VAL	= false
	config.WARN_AS_ERROR			= false
	config.WARN_FORMAT				= "$file:$line: $text"
	config.WARN_LINE_FORMAT			= "at line $line of file $file"
	config.WARN_LOGFILE				= ""
	config.INPUT					= {"pages",
						 "../ExamMspaint/ExamPaint/include"}
	config.INPUT_ENCODING			= "UTF-8"
	config.INPUT_FILE_ENCODING		= ""
	config.FILE_PATTERNS			= {"*.c",
						 "*.cc",
						 "*.cxx",
						 "*.cpp",
						 "*.c++",
						 "*.java",
						 "*.ii",
						 "*.ixx",
						 "*.ipp",
						 "*.i++",
						 "*.inl",
						 "*.idl",
						 "*.ddl",
						 "*.odl",
						 "*.h",
						 "*.hh",
						 "*.hxx",
						 "*.hpp",
						 "*.h++",
						 "*.l",
						 "*.cs",
						 "*.d",
						 "*.php",
						 "*.php4",
						 "*.php5",
						 "*.phtml",
						 "*.inc",
						 "*.m",
						 "*.markdown",
						 "*.md",
						 "*.mm",
						 "*.dox",
						 "*.py",
						 "*.pyw",
						 "*.f90",
						 "*.f95",
						 "*.f03",
						 "*.f08",
						 "*.f18",
						 "*.f",
						 "*.for",
						 "*.vhd",
						 "*.vhdl",
						 "*.ucf",
						 "*.qsf",
						 "*.ice",
						 "*.txt"}
	config.RECURSIVE				= true
	config.EXCLUDE					= ""
	config.EXCLUDE_SYMLINKS			= false
	config.EXCLUDE_PATTERNS			= ""
	config.EXCLUDE_SYMBOLS			= ""
	config.EXAMPLE_PATH				= "examples"
	config.EXAMPLE_PATTERNS			= "*"
	config.EXAMPLE_RECURSIVE		= false
	config.IMAGE_PATH				= "resources"
	config.INPUT_FILTER				= ""
	config.FILTER_PATTERNS			= ""
	config.FILTER_SOURCE_FILES		= false
	config.FILTER_SOURCE_PATTERNS	= ""
	config.USE_MDFILE_AS_MAINPAGE	= ""
	config.FORTRAN_COMMENT_AFTER	= 72
	config.SOURCE_BROWSER			= true
	config.INLINE_SOURCES			= false
	config.STRIP_CODE_COMMENTS		= true
	config.REFERENCED_BY_RELATION	= false
	config.REFERENCES_RELATION		= false
	config.REFERENCES_LINK_SOURCE	= true
	config.SOURCE_TOOLTIPS			= true
	config.USE_HTAGS				= false
	config.VERBATIM_HEADERS			= true
	config.CLANG_ASSISTED_PARSING	= false
	config.CLANG_ADD_INC_PATHS		= true
	config.CLANG_OPTIONS			= ""
	config.CLANG_DATABASE_PATH		= ""
	config.ALPHABETICAL_INDEX		= true
	config.IGNORE_PREFIX			= ""
	config.GENERATE_HTML			= true
	config.HTML_OUTPUT				= "html"
	config.HTML_FILE_EXTENSION		= ".html"
	config.HTML_HEADER				= ""
	config.HTML_FOOTER				= ""
	config.HTML_STYLESHEET			= ""
	config.HTML_EXTRA_STYLESHEET	= ""
	config.HTML_EXTRA_FILES			= ""
	config.HTML_COLORSTYLE			= "AUTO_LIGHT"
	config.HTML_COLORSTYLE_HUE		= 220
	config.HTML_COLORSTYLE_SAT		= 100
	config.HTML_COLORSTYLE_GAMMA	= 80
	config.HTML_DYNAMIC_MENUS		= true
	config.HTML_DYNAMIC_SECTIONS	= false
	config.HTML_CODE_FOLDING		= true
	config.HTML_COPY_CLIPBOARD		= true
	config.HTML_PROJECT_COOKIE		= ""
	config.HTML_INDEX_NUM_ENTRIES	= 100
	config.GENERATE_DOCSET			= false
	config.DOCSET_FEEDNAME			= "Doxygen generated docs"
	config.DOCSET_FEEDURL			= ""
	config.DOCSET_BUNDLE_ID			= "org.doxygen.Project"
	config.DOCSET_PUBLISHER_ID		= "org.doxygen.Publisher"
	config.DOCSET_PUBLISHER_NAME	= "Publisher"
	config.GENERATE_HTMLHELP		= false
	config.CHM_FILE					= ""
	config.HHC_LOCATION				= ""
	config.GENERATE_CHI				= false
	config.CHM_INDEX_ENCODING		= ""
	config.BINARY_TOC				= false
	config.TOC_EXPAND				= true
	config.SITEMAP_URL				= ""
	config.GENERATE_QHP				= false
	config.QCH_FILE					= ""
	config.QHP_NAMESPACE			= "org.doxygen.Project"
	config.QHP_VIRTUAL_FOLDER		= "doc"
	config.QHP_CUST_FILTER_NAME		= ""
	config.QHP_CUST_FILTER_ATTRS	= ""
	config.QHP_SECT_FILTER_ATTRS	= ""
	config.QHG_LOCATION				= ""
	config.GENERATE_ECLIPSEHELP		= false
	config.ECLIPSE_DOC_ID			= "org.doxygen.Project"
	config.DISABLE_INDEX			= false
	config.GENERATE_TREEVIEW		= true
	config.FULL_SIDEBAR				= false
	config.ENUM_VALUES_PER_LINE		= 4
	config.SHOW_ENUM_VALUES			= false
	config.TREEVIEW_WIDTH			= 250
	config.EXT_LINKS_IN_WINDOW		= false
	config.OBFUSCATE_EMAILS			= true
	config.HTML_FORMULA_FORMAT		= "svg"
	config.FORMULA_FONTSIZE			= 10
	config.FORMULA_MACROFILE		= ""
	config.USE_MATHJAX				= false
	config.MATHJAX_VERSION			= "MathJax_2"
	config.MATHJAX_FORMAT			= "HTML-CSS"
	config.MATHJAX_RELPATH			= ""
	config.MATHJAX_EXTENSIONS		= ""
	config.MATHJAX_CODEFILE			= ""
	config.SEARCHENGINE				= true
	config.SERVER_BASED_SEARCH		= false
	config.EXTERNAL_SEARCH			= false
	config.SEARCHENGINE_URL			= ""
	config.SEARCHDATA_FILE			= "searchdata.xml"
	config.EXTERNAL_SEARCH_ID		= ""
	config.EXTRA_SEARCH_MAPPINGS	= ""
	config.GENERATE_LATEX			= false
	config.LATEX_OUTPUT				= "latex"
	config.LATEX_CMD_NAME			= ""
	config.MAKEINDEX_CMD_NAME		= "makeindex"
	config.LATEX_MAKEINDEX_CMD		= "makeindex"
	config.COMPACT_LATEX			= false
	config.PAPER_TYPE				= "a4"
	config.EXTRA_PACKAGES			= ""
	config.LATEX_HEADER				= ""
	config.LATEX_FOOTER				= ""
	config.LATEX_EXTRA_STYLESHEET	= ""
	config.LATEX_EXTRA_FILES		= ""
	config.PDF_HYPERLINKS			= true
	config.USE_PDFLATEX				= true
	config.LATEX_BATCHMODE			= false
	config.LATEX_HIDE_INDICES		= false
	config.LATEX_BIB_STYLE			= "plain"
	config.LATEX_EMOJI_DIRECTORY	= ""
	config.GENERATE_RTF				= false
	config.RTF_OUTPUT				= "rtf"
	config.COMPACT_RTF				= false
	config.RTF_HYPERLINKS			= false
	config.RTF_STYLESHEET_FILE		= ""
	config.RTF_EXTENSIONS_FILE		= ""
	config.RTF_EXTRA_FILES			= ""
	config.GENERATE_MAN				= false
	config.MAN_OUTPUT				= "man"
	config.MAN_EXTENSION			= .3
	config.MAN_SUBDIR				= ""
	config.MAN_LINKS				= false
	config.GENERATE_XML				= true
	config.XML_OUTPUT				= "xml"
	config.XML_PROGRAMLISTING		= true
	config.XML_NS_MEMB_FILE_SCOPE	= false
	config.GENERATE_DOCBOOK			= false
	config.DOCBOOK_OUTPUT			= "docbook"
	config.GENERATE_AUTOGEN_DEF		= false
	config.GENERATE_SQLITE3			= false
	config.SQLITE3_OUTPUT			= "sqlite3"
	config.SQLITE3_RECREATE_DB		= true
	config.GENERATE_PERLMOD			= false
	config.PERLMOD_LATEX			= false
	config.PERLMOD_PRETTY			= true
	config.PERLMOD_MAKEVAR_PREFIX	= ""
	config.ENABLE_PREPROCESSING		= true
	config.MACRO_EXPANSION			= false
	config.EXPAND_ONLY_PREDEF		= false
	config.SEARCH_INCLUDES			= true
	config.INCLUDE_PATH				= ""
	config.INCLUDE_FILE_PATTERN		= ""
	config.PREDEFINED				= ""
	config.EXPAND_AS_DEFINED		= ""
	config.SKIP_FUNCTION_MACROS		= true
	config.TAGFILES					= ""
	config.GENERATE_TAGFILE			= ""
	config.ALLEXTERNALS				= false
	config.EXTERNAL_GROUPS			= true
	config.EXTERNAL_PAGES			= true
	config.HIDE_UNDOC_RELATIONS		= true
	config.HAVE_DOT					= true
	config.DOT_NUM_THREADS			= 0
	config.DOT_COMMON_ATTR			= "fontname=Helvetica,fontsize=10"
	config.DOT_EDGE_ATTR			= "labelfontname=Helvetica,labelfontsize=10"
	config.DOT_NODE_ATTR			= "shape=box,height=0.2,width=0.4"
	config.DOT_FONTPATH				= ""
	config.CLASS_GRAPH				= true
	config.COLLABORATION_GRAPH		= true
	config.GROUP_GRAPHS				= true
	config.UML_LOOK					= false
	config.UML_LIMIT_NUM_FIELDS		= 10
	config.DOT_UML_DETAILS			= false
	config.DOT_WRAP_THRESHOLD		= 17
	config.TEMPLATE_RELATIONS		= false
	config.INCLUDE_GRAPH			= true
	config.INCLUDED_BY_GRAPH		= true
	config.CALL_GRAPH				= true
	config.CALLER_GRAPH				= true
	config.GRAPHICAL_HIERARCHY		= true
	config.DIRECTORY_GRAPH			= false
	config.DIR_GRAPH_MAX_DEPTH		= 1
	config.DOT_IMAGE_FORMAT			= "svg"
	config.INTERACTIVE_SVG			= false
	config.DOT_PATH					= "D:/Project/TestDrive/bin/msys64/ucrt64/bin"
	config.DOTFILE_DIRS				= ""
	config.DIA_PATH					= ""
	config.DIAFILE_DIRS				= ""
	config.PLANTUML_JAR_PATH		= ""
	config.PLANTUML_CFG_FILE		= ""
	config.PLANTUML_INCLUDE_PATH	= ""
	config.DOT_GRAPH_MAX_NODES		= 50
	config.MAX_DOT_GRAPH_DEPTH		= 0
	config.DOT_MULTI_TARGETS		= false
	config.GENERATE_LEGEND			= true
	config.DOT_CLEANUP				= true
	config.MSCGEN_TOOL				= ""
	config.MSCFILE_DIRS				= ""
end