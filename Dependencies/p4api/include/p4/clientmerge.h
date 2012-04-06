/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

#ifndef __CLIENTMERGE__
#define __CLIENTMERGE__

/*
 * ClientMerge - client side merge controller
 *
 * ClientMerge
 *
 *	A ClientMerge object handles the client-side merge process,
 *	taking the server-generated merge stream and writing out the
 *	base, yours, theirs, and merge files.  A ClientMerge also has 
 *	both a command line resolve implementation as well as hooks 
 *	to allow other resolve implementations.
 *
 *	ClientMerge is almost purely virtual, allowing for varying 
 *	implementations that deal with the varying server-generated 
 *	merge streams (basically 2 and 3 way merges).
 *
 *	Half of ClientMerge's methods are for handling the server merge 
 *	stream; the other half are for the user interface.
 *
 * Virtual member functions - Server merge stream handling
 *
 *	ClientMerge::SetNames() - provides the user-recognisable names
 *		for the base, theirs, and yours.  
 *
 *	ClientMerge::SetShowAll() - turns on verbose merging.
 *
 *	ClientMerge::Open() - state name of client file and prepare to
 *		process merge pipe.
 *
 *	ClientMerge::Write() - write a block of the merge pipe.  See
 *		diffmerge.h for the meaning of bits.
 *
 *	ClientMerge::Close() - close file(s) at end of merge pipe.
 *
 *	ClientMerge::Select() - move user-selected result into place
 *
 *	ClientMerge::Chmod() - set permissions on the target file;
 *		generally, set to rw before and ro after.
 *
 * Virtual member functions - User interface hooks
 *
 *	ClientMerge::AutoResolve() - take a guess at which version 
 *		(theirs, yours, result) should be the result of the
 *		merge, using the chunk counts as hints.
 *
 *	ClientMerge::Resolve() - let the user select which version 
 *		should be the result of the merge.
 *
 *	ClientMerge::DetectResolve() - determine which version by 
 *		comparing result vs theirs/yours/merged.
 *
 *	ClientMerge::IsAcceptable() - returns 1 if the result file
 *		has no merge markers (generated by the merge stream
 *		handler) left in it.
 *
 *	ClientMerge::GetBaseFile()
 *	ClientMerge::GetYourFile()
 *	ClientMerge::GetTheirFile()
 *	ClientMerge::GetResultFile()
 *		Return a FileSys * to the desired file.  2 way merges
 *		return 0 for Base/Result files: only Yours/Theirs is
 *		available.
 *
 *	ClientMerge::GetYourChunks()
 *	ClientMerge::GetTheirChunks()
 *	ClientMerge::GetBothChunks()
 *	ClientMerge::GetConflictChunks()
 *		Returns the number of chunks in the merge stream.
 *		2 way merges return 0 for all.
 *
 * The actual caller of the ClientMerge class is in clientservice.cc.
 * It uses the stream handling functions to produce 2 or 4 files on
 * the client (yours/theirs, yours/theirs/base/result), and then calls
 * 
 *	MergeType ClientUser::Resolve( ClientMerge *merger )
 *
 * The default ClientUser::Resolve() merely calls merger->Resolve()
 * to carry out the command-line resolve interaction, but a subclassed
 * ClientUser::Resolve() can use the other merger methods to gain 
 * access to the files and performs its own resolve.
 */

enum MergeType {
	CMT_BINARY,	// binary merge
	CMT_3WAY, 	// 3-way text 
	CMT_2WAY	// 2-way text
} ;

enum MergeStatus {
	CMS_QUIT,	// user wants to quit
	CMS_SKIP,	// skip the integration record
	CMS_MERGED,	// accepted merged theirs and yours
	CMS_EDIT,	// accepted edited merge
	CMS_THEIRS,	// accepted theirs
	CMS_YOURS	// accepted yours,
} ;

enum MergeForce {
	CMF_AUTO,	// don't force			// -am
	CMF_SAFE,	// accept only non-conflicts	// -as
	CMF_FORCE	// accept anything		// -af
} ;

class ClientUser;

class ClientMerge : public LastChance {

    public:
	static ClientMerge *Create( 
			    ClientUser *ui, 
			    FileSysType type,
			    FileSysType resType,
			    MergeType m );

	static ClientMerge *Create( 
			    ClientUser *ui, 
			    FileSysType type,
			    FileSysType resType,
			    FileSysType theirType,
			    FileSysType baseType,
			    MergeType m );

	virtual		~ClientMerge();

	// User interface: AutoResolve is called if any of the -a flags
	// are passed to 'p4 resolve'.  Resolve() is used otherwise.  The
	// Resolve()'s often call AutoResolve() to provide the user with 
	// a default selection.

	virtual MergeStatus AutoResolve( MergeForce forceMerge ) = 0;
	virtual MergeStatus Resolve( Error *e ) = 0;
	virtual MergeStatus DetectResolve() const = 0;

	virtual int	IsAcceptable() const = 0;

	virtual FileSys *GetBaseFile() const = 0;
	virtual FileSys *GetYourFile() const = 0;
	virtual FileSys *GetTheirFile() const = 0;
	virtual FileSys *GetResultFile() const = 0;

	virtual int	GetYourChunks() const = 0;
	virtual int	GetTheirChunks() const = 0;
	virtual int	GetBothChunks() const = 0;
	virtual int	GetConflictChunks() const = 0;

	virtual const StrPtr *GetMergeDigest() const { return NULL; }
	virtual const StrPtr *GetYourDigest() const { return NULL; }
	virtual const StrPtr *GetTheirDigest() const { return NULL; }

	// Server merge stream handling

	virtual void	SetNames( StrPtr *, StrPtr *, StrPtr * ) {};
	virtual void	CopyDigest( StrPtr *, Error * ) {};
	virtual void	SetShowAll() {};
	virtual void	SetDiffFlags( const StrPtr * ) {};

	virtual void	Open( StrPtr *name, Error *e, CharSetCvt * = 0,
				int charset = 0 ) = 0;
	virtual void	Write( StrPtr *buf, StrPtr *bits, Error *e ) = 0;
	virtual void	Close( Error *e ) = 0;
	virtual void	Select( MergeStatus stat, Error *e ) = 0;
	virtual void	Chmod( const char *perms, Error *e ) = 0;

	// Chmod must use 'const char' as the type not 'char'
	// The following will cause compile time errors for using 'char'

	virtual int	Chmod( char *perms, Error *e )
	    { Chmod( (const char *)perms, e ); return 0; };

    protected:

	ClientUser	*ui;
	static const char *const confirm;	// confirm overwrite

	int 		Verify( const Error *message, Error *e );

} ;

# endif /* __CLIENTMERGE__ */
