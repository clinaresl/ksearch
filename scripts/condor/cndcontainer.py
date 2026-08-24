#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# cndcontainer.py
# Description:
# -----------------------------------------------------------------------------
#
# Started on <mar 18-08-2026 14:15:30.015445970 (1787055330)>
# Carlos Linares López <carlos.linares@uc3m.es>
#

"""
Associates metadata to a bundle of files
"""

# imports
# -----------------------------------------------------------------------------
import subprocess
from pathlib import Path

import cndmetadata
import cndbundle
import utils

# globals
# -----------------------------------------------------------------------------

# default logger
LOGGER = utils.LOGGER

# info
NUMBER_OF_BUNDLES = "# Bundles: {}"
NUMBER_OF_FILES = "# Files: {} in {} bundles"

# warnings
WARNING_INCOMPLETE_BUNDLE = "The bundle '{}' is incomplete!"

# errors
ERROR_INVALID_BUNDLE = "The bundle '{}' is invalid!"

# errors
ERROR_NO_METADATA = "It is not possible to extract any metadata from file '{}'"


# -----------------------------------------------------------------------------
# CNDContainer
#
# indexes bundles by their metadata and provide services for them
# -----------------------------------------------------------------------------
class CNDContainer:
    """indexes bundles by their metadata and provide services for them

    """

    def __init__(self):
        """A container is created empty and files are added to it

        """

        # initialize the container as a dictionary of metadata to bundles
        self._bundles: dict[cndmetadata.CNDMetadata, cndbundle.CNDBundle] = {}


    def __iadd__(self, filepath: Path):
        """adds a file to this container properly indexed by its metadata"""

        # check whether this file contains metadata and, if so, retrieve it
        if (metadata := cndbundle.validate_metadata(filepath)) is not None:

            # check whether this metadata has been previously stored in this
            # container
            if metadata not in self._bundles:

                # if it has never been stored, then create an empty bundle
                self._bundles[metadata] = cndbundle.CNDBundle()

            # and now add the file
            self._bundles[metadata] += filepath

        else:

            LOGGER.critical(ERROR_NO_METADATA.format(filepath))
            raise ValueError(ERROR_NO_METADATA.format(filepath))

        return self

    def __str__(self) -> str:
        """provide a human-readable version of the contents of this container"""

        # -- initialization
        output = "\n\n"
        
        # for all metadata
        for imetadata in self._bundles:

            output += f" {imetadata}:\n"
            
            # and all files in each bundle
            for ifile in self._bundles[imetadata]:
                output += f"\t - {ifile.name}\n"

            output += f"\t Complete: {self._bundles[imetadata].is_complete()}\n"
            output += f"\t Valid   : {self._bundles[imetadata].is_valid()}\n\n"

        return output

    def get_nb_bundles(self) -> int:
        """return the total number of bundles in this container"""

        return len(self._bundles)

    def get_nb_valid_bundles(self) -> int:
        """return the total number of valid bundles in this container"""

        # just count the number of valid bundles
        nb_valid_bundles = 0
        for ibundle in self._bundles:
            if self._bundles[ibundle].is_valid():
                nb_valid_bundles += 1

        return nb_valid_bundles
    
    def get_nb_complete_bundles(self) -> int:
        """return the total number of complete bundles in this container"""

        # just count the number of valid bundles
        nb_complete_bundles = 0
        for ibundle in self._bundles:
            if self._bundles[ibundle].is_complete():
                nb_complete_bundles += 1

        return nb_complete_bundles

    def list(self,
             all: bool, 
             valid: bool, 
             invalid: bool, 
             complete: bool, 
             incomplete: bool, 
             complete_and_valid: bool,
             incomplete_and_valid: bool,
             complete_and_invalid: bool,
             incomplete_and_invalid: bool,
             domain: str,
             project: str,
             algorithm: str,
             variant: str,
             size: str):
        """show all bundles that satisfy the given criteria and belong to any of
           the given fields

        """

        # --initialization
        nbbundles = 0
        
        # access each bundle and its metadata
        for imetadata, ibundle in self._bundles.items():

            # check whether this specific bundle has to be listed or not
            if all or \
               (valid and ibundle.is_valid()) or \
               (invalid and not ibundle.is_valid()) or \
               (complete and ibundle.is_complete()) or \
               (incomplete and not ibundle.is_complete()) or \
               (complete_and_valid and ibundle.is_complete() and ibundle.is_valid()) or \
               (incomplete_and_valid and not ibundle.is_complete() and ibundle.is_valid()) or \
               (complete_and_invalid and ibundle.is_complete() and not ibundle.is_valid()) or \
               (incomplete_and_invalid and not ibundle.is_complete() and not ibundle.is_valid()):

                # if any field is given check it matches
                if (domain is None or imetadata.get_domain() == domain) and \
                   (project is None or imetadata.get_project() == project) and \
                   (algorithm is None or imetadata.get_algorithm() == algorithm) and \
                   (variant is None or imetadata.get_variant() == variant) and \
                   (size is None or imetadata.get_size() == size):
                
                    # then show this bundle
                    nbbundles += 1
                    ibundle.list(imetadata)

        # show the number of bundles satisfying the given criteria
        LOGGER.info(NUMBER_OF_BUNDLES.format(nbbundles))

    def move(self, dry_run: bool, force: bool):
        """move all files of all valid and complete bundles to their
           corresponding directories. If dry_run is given, a message informing
           the action to take is shown and no action is taken. If force is given
           data is overwritten if necessary.

        """

        # get access to each bundle
        for imetadata in self._bundles:

            # if this bundle is complete
            if self._bundles[imetadata].is_valid():

                # and it is also complete:
                if self._bundles[imetadata].is_complete():

                    # then move it
                    self._bundles[imetadata].move(imetadata, dry_run, force)

                else:

                    # in case it is not complete then issue a warning
                    LOGGER.warning(WARNING_INCOMPLETE_BUNDLE.format(imetadata))

            else:

                # in case it is not valid issue an error
                LOGGER.error(ERROR_INVALID_BUNDLE.format(imetadata))        


    def remove(self,
               dry_run:bool,
               remove_all: bool, 
               remove_valid: bool, 
               remove_invalid: bool, 
               remove_complete: bool, 
               remove_incomplete: bool, 
               remove_complete_and_valid: bool,
               remove_incomplete_and_valid: bool,
               remove_complete_and_invalid: bool,
               remove_incomplete_and_invalid: bool,
               domain: str,
               project: str,
               algorithm: str,
               variant: str,
               size: str):
        """remove all files from a bundle that satisfy the given criteria and
           belong to any of the given fields in the given order

        """

        # access each bundle and its metadata
        for imetadata, ibundle in self._bundles.items():

            # check whether this specific bundle has to be removed or not
            if remove_all or \
               (remove_valid and ibundle.is_valid()) or \
               (remove_invalid and not ibundle.is_valid()) or \
               (remove_complete and ibundle.is_complete()) or \
               (remove_incomplete and not ibundle.is_complete()) or \
               (remove_complete_and_valid and ibundle.is_complete() and ibundle.is_valid()) or \
               (remove_incomplete_and_valid and not ibundle.is_complete() and ibundle.is_valid()) or \
               (remove_complete_and_invalid and ibundle.is_complete() and not ibundle.is_valid()) or \
               (remove_incomplete_and_invalid and not ibundle.is_complete() and not ibundle.is_valid()):

                # if any field is given check it matches
                if (domain is None or imetadata.get_domain() == domain) and \
                   (project is None or imetadata.get_project() == project) and \
                   (algorithm is None or imetadata.get_algorithm() == algorithm) and \
                   (variant is None or imetadata.get_variant() == variant) and \
                   (size is None or imetadata.get_size() == size):
                
                    # then remove the files of this bundle
                    ibundle.remove(dry_run)

    def tar(self,
            archive:Path,
            all: bool, 
            valid: bool, 
            invalid: bool, 
            complete: bool, 
            incomplete: bool, 
            complete_and_valid: bool,
            incomplete_and_valid: bool,
            complete_and_invalid: bool,
            incomplete_and_invalid: bool,
            domain: str,
            project: str,
            algorithm: str,
            variant: str,
            size: str):
        """compress all files from all bundles that satisfy the given criteria
           and belong to any of the given fields in the given archive

        """

        # --initialization
        nbbundles = 0
        files = []
        
        # access each bundle and its metadata
        for imetadata, ibundle in self._bundles.items():

            # check whether this specific bundle has to be listed or not
            if all or \
               (valid and ibundle.is_valid()) or \
               (invalid and not ibundle.is_valid()) or \
               (complete and ibundle.is_complete()) or \
               (incomplete and not ibundle.is_complete()) or \
               (complete_and_valid and ibundle.is_complete() and ibundle.is_valid()) or \
               (incomplete_and_valid and not ibundle.is_complete() and ibundle.is_valid()) or \
               (complete_and_invalid and ibundle.is_complete() and not ibundle.is_valid()) or \
               (incomplete_and_invalid and not ibundle.is_complete() and not ibundle.is_valid()):

                # if any field is given check it matches
                if (domain is None or imetadata.get_domain() == domain) and \
                   (project is None or imetadata.get_project() == project) and \
                   (algorithm is None or imetadata.get_algorithm() == algorithm) and \
                   (variant is None or imetadata.get_variant() == variant) and \
                   (size is None or imetadata.get_size() == size):
                
                    # then all files from this bundle to the list of files to
                    # compress
                    nbbundles += 1
                    for ifile in ibundle:
                        files.append(ifile.name)

        # and now compress data with tar
        subprocess.run(
            ["tar", "-czvf", archive, "--", *files],
            check=True,
        )
                        
        # show the number of bundles satisfying the given criteria
        LOGGER.info(NUMBER_OF_FILES.format(len(files), nbbundles))


# Local Variables:
# mode:python
# fill-column:80
# End:
