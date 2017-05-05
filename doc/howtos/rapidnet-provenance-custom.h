/*!
\page rapidnet-provenance-custom How can we customize the provenance implementation?
\anchor rapidnet-provenance-custom

<b>Question:</b> How can we customize the provenance implementation?

<b>Answer:</b> The provenance query implementation in RapidNet can be customized in the
following ways:

  - The representation of the provenance information of EDB tuples: The default function
    f_pEDB returns the provenance id. This can be customized with a different implementation.

  - The representation of the provenance information of IDB tuples. The default function
    f_pIDB connects all the rule derivations of a given tuple using a "+" to concatenate them.

  - The representation of the provenance information corresponding to rule execution.
    The default method f_pRule returns the rule id, rule location, and uses "*"
    to concatenate the two tuples used in a single rule.

Note that in the above cases, you can change the implementation in several ways. By either
rewriting the function, or writing a new function and binding it to the same name f_pEDB, f_pIDB
etc in the compiler or writing a new function and binding it to a new name. It is recommended
to not bind to a new name as it may need code changes in the provenance rewrite feature.

The provenance maintenance in RapidNet can be customized in one way:

  - You can customize the method used to generate a tuple ID. The default is to use f_sha1.
    This customization may need compiler code changes. You need to make sure that this change
    should be consistent with any customizations in the query implementation.
*/

