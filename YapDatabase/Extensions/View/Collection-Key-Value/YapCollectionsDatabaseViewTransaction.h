#import <Foundation/Foundation.h>
#import "YapAbstractDatabaseExtensionTransaction.h"


@interface YapCollectionsDatabaseViewTransaction : YapAbstractDatabaseExtensionTransaction

- (NSUInteger)numberOfGroups;
- (NSArray *)allGroups;

- (NSUInteger)numberOfKeysInGroup:(NSString *)group;
- (NSUInteger)numberOfKeysInAllGroups;

- (BOOL)getKey:(NSString **)keyPtr
    collection:(NSString **)collectionPtr
       atIndex:(NSUInteger)index
       inGroup:(NSString *)group;

- (NSString *)groupForKey:(NSString *)key inCollection:(NSString *)collection;

- (BOOL)getGroup:(NSString **)groupPtr
           index:(NSUInteger *)indexPtr
          forKey:(NSString *)key
    inCollection:(NSString *)collection;


- (void)enumerateKeysInGroup:(NSString *)group
                  usingBlock:(void (^)(NSString *collection, NSString *key, NSUInteger index, BOOL *stop))block;

- (void)enumerateKeysInGroup:(NSString *)group
                 withOptions:(NSEnumerationOptions)options
                  usingBlock:(void (^)(NSString *collection, NSString *key, NSUInteger index, BOOL *stop))block;

- (void)enumerateKeysInGroup:(NSString *)group
                 withOptions:(NSEnumerationOptions)options
                       range:(NSRange)range
                  usingBlock:(void (^)(NSString *collection, NSString *key, NSUInteger index, BOOL *stop))block;

@end

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * YapCollectionsDatabaseView deals with ordered arrays of collection/key tuples.
 * So, strictly speaking, it only knows about collection/key tuples, groups, and indexes.
 * 
 * But it's really convenient to have methods that put it all together to fetch an object in a single method.
**/
@interface YapCollectionsDatabaseViewTransaction (Convenience)

/**
 * Equivalent to invoking:
 *
 * NSString *collection = nil;
 * NSString *key = nil;
 * [[transaction ext:@"myView"] getKey:&key collection:&collection atIndex:index inGroup:group];
 * [transaction objectForKey:key inColleciton:collection];
**/
- (id)objectAtIndex:(NSUInteger)keyIndex inGroup:(NSString *)group;

/**
 * The following methods are equivalent to invoking the enumerateKeysInGroup:... methods,
 * and then fetching the metadata within your own block.
**/

- (void)enumerateKeysAndMetadataInGroup:(NSString *)group
                             usingBlock:
                    (void (^)(NSString *collection, NSString *key, id metadata, NSUInteger index, BOOL *stop))block;

- (void)enumerateKeysAndMetadataInGroup:(NSString *)group
                            withOptions:(NSEnumerationOptions)options
                             usingBlock:
                    (void (^)(NSString *collection, NSString *key, id metadata, NSUInteger index, BOOL *stop))block;

- (void)enumerateKeysAndMetadataInGroup:(NSString *)group
                            withOptions:(NSEnumerationOptions)options
                                  range:(NSRange)range
                             usingBlock:
                    (void (^)(NSString *collection, NSString *key, id metadata, NSUInteger index, BOOL *stop))block;

/**
 * The following methods are equivalent to invoking the enumerateKeysInGroup:... methods,
 * and then fetching the object and metadata within your own block.
**/

- (void)enumerateKeysAndObjectsInGroup:(NSString *)group
                            usingBlock:
            (void (^)(NSString *collection, NSString *key, id object, id metadata, NSUInteger index, BOOL *stop))block;

- (void)enumerateKeysAndObjectsInGroup:(NSString *)group
                           withOptions:(NSEnumerationOptions)options
                            usingBlock:
            (void (^)(NSString *collection, NSString *key, id object, id metadata, NSUInteger index, BOOL *stop))block;

- (void)enumerateKeysAndObjectsInGroup:(NSString *)group
                           withOptions:(NSEnumerationOptions)options
                                 range:(NSRange)range
                            usingBlock:
            (void (^)(NSString *collection, NSString *key, id object, id metadata, NSUInteger index, BOOL *stop))block;

@end

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * The following methods have to do with the configuration of the internal workings of the view.
**/
@interface YapCollectionsDatabaseViewTransaction (Configuration)

/**
 * The view is tasked with storing ordered arrays of keys.
 * In doing so, it splits the array into "pages" of keys,
 * and stores the pages in the database.
 * This reduces disk IO, as only the contents of a single page are written for a single change.
 * And only the contents of a single page need be read to fetch a single key.
 *
 * The default pageSize if 50.
 * That is, the view will split up arrays into groups of up to 50 keys,
 * and store each as a separate page.
**/
- (NSUInteger)pageSize;

/**
 * Allows you to configure the pageSize.
 *
 * Note: Changing the pageSize for an active view may cause some IO as
 *       the view may need to restructure its existing pages.
 *
 * This method only works from within a readwrite transaction.
 * Invoking this method from within a readonly transaction does nothing.
**/
- (void)setPageSize:(NSUInteger)pageSize;

@end