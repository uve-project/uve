// This defines the interface to the QsciLexerSystemverilog class.
//
// Copyright (c) 2011 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public
// License versions 2.0 or 3.0 as published by the Free Software
// Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
// included in the packaging of this file.  Alternatively you may (at
// your option) use any later version of the GNU General Public
// License if such license has been publicly approved by Riverbank
// Computing Limited (or its successors, if any) and the KDE Free Qt
// Foundation. In addition, as a special exception, Riverbank gives you
// certain additional rights. These rights are described in the Riverbank
// GPL Exception version 1.1, which can be found in the file
// GPL_EXCEPTION.txt in this package.
// 
// If you are unsure which license is appropriate for your use, please
// contact the sales department at sales@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#ifndef QSCILEXERDYSTEMVERILOG_H
#define QSCILEXERDYSTEMVERILOG_H
#ifdef __APPLE__
extern "C++" {
#endif

#include <qobject.h>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The QsciLexerSystemsystemverilog class encapsulates the Scintilla SystemVerilog lexer.
class QSCINTILLA_EXPORT QsciLexerSystemVerilog : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! SystemVerilog lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A line comment.
        CommentLine = 2,

        //! A bang comment.
        CommentBang = 3,

        //! A number
        Number = 4,

        //! A keyword.
		//! A keyword defined in keyword set number 1.
        Keyword = 5,

        //! A string.
		//! A keyword defined in keyword set number 2.
        String = 6,

        //! A keyword defined in keyword set number 3.
        KeywordSet2 = 7,

		//! all uvm class
		//! A keyword defined in keyword set number 4.
        KeywordUvmClass = 8,
		
		//! all uvm method
		//! A keyword defined in keyword set number 5.
        KeywordUvmMethod = 9,
		 
		//! all uvm macro 
		//! A keyword defined in keyword set number 6.
        KeywordUvmMacro = 10,	
		
        //! A pre-processor block.
        Preprocessor = 11,

        //! An operator.
        Operator = 12,

        //! An identifier.
        Identifier = 13,

        //! The end of a line where a string is not closed.
        UnclosedString = 14
    };

    //! Construct a QsciLexerSystemVerilog with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerSystemVerilog(QObject *parent = 0);


    //! Destroys the QsciLexerSystemVerilog instance.
    virtual ~QsciLexerSystemVerilog();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! \internal Returns the style used for braces for brace matching.
    int braceStyle() const;

	//! \internal Returns the string of characters that comprise a word.
    const char *wordCharacters() const;
    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the end-of-line fill for style number \a style.
    bool defaultEolFill(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the background colour of the text for style number \a style.
    //!
    //! \sa defaultColor()
    QColor defaultPaper(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    void refreshProperties();

    //! If \a fold is true then "} else {" lines can be folded.  The
    //! default is false.
    //!
    //! \sa foldAtElse()
    void setFoldAtElse(bool fold);

    //! Returns true if "} else {" lines can be folded.
    //!
    //! \sa setFoldAtElse()
    bool foldAtElse() const;

    //! If \a fold is true then multi-line comment blocks can be folded.
    //! The default is false.
    //!
    //! \sa foldComments()
    void setFoldComments(bool fold);

    //! Returns true if multi-line comment blocks can be folded.
    //!
    //! \sa setFoldComments()
    bool foldComments() const;

    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    void setFoldCompact(bool fold);

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const;

    //! If \a fold is true then preprocessor blocks can be folded.  The
    //! default is true.
    //!
    //! \sa foldPreprocessor()
    void setFoldPreprocessor(bool fold);

    //! Returns true if preprocessor blocks can be folded.
    //!
    //! \sa setFoldPreprocessor()
    bool foldPreprocessor() const;

    //! If \a fold is true then modules can be folded.  The default is false.
    //!
    //! \sa foldAtModule()
    void setFoldAtModule(bool fold);

    //! Returns true if modules can be folded.
    //!
    //! \sa setFoldAtModule()
    bool foldAtModule() const;

protected:
    //! The lexer's properties are read from the settings \a qs.  \a prefix
    //! (which has a trailing '/') should be used as a prefix to the key of
    //! each setting.  true is returned if there is no error.
    //!
    //! \sa writeProperties()
    bool readProperties(QSettings &qs,const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    //! \sa readProperties()
    bool writeProperties(QSettings &qs,const QString &prefix) const;

private:
    void setAtElseProp();
    void setCommentProp();
    void setCompactProp();
    void setPreprocProp();
    void setAtModuleProp();

    bool fold_atelse;
    bool fold_comments;
    bool fold_compact;
    bool fold_preproc;
    bool fold_atmodule;

    QsciLexerSystemVerilog(const QsciLexerSystemVerilog &);
    QsciLexerSystemVerilog &operator=(const QsciLexerSystemVerilog &);
};

#ifdef __APPLE__
}
#endif

#endif
